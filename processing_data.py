#importing libraries
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import savgol_filter
from scipy.optimize import curve_fit
import math
import scipy.ndimage as ndimage
import serial
from openpyxl import Workbook


# Initialize serial port
ser = serial.Serial('COM4', 9600)  # Example: COM4 is the port, 9600 is the baud rate

num_samples = 16
time_master = []
aceel_x_master = []
aceel_y_master = []
data_final = []

for _ in range(num_samples):
    data = ser.readline().decode('latin-1').strip().split()  # Assuming data is space-separated
    if len(data) >= 3:  # Assuming you have at least three values in data
        time_master.append(float(data[0]))
        aceel_x_master.append(float(data[1]))
        aceel_y_master.append(float(data[2]))
        data_final.append(data)
        print(data)

# Close serial port when done
ser.close()

wb = Workbook()
ws = wb.active

# Writing header row
header = ["V", "Irr", "I"]
ws.append(header)

# Writing data rows
for row in data_final:
    ws.append(row)

wb.save("data.xlsx")

# Read Resistance from Excel file
R = pd.read_excel("R.xlsx")
R = R["R"][:]
#reading the data file
data = pd.read_excel("data.xlsx")
# Extract resistance and voltage data
V = data["V"][:]
Irr_R_arr = data["Irr"][:]
I_sensor=data["I"][:]

# Calculate current
I = V / R

#calculating irradiaance
Irr_R=np.mean(Irr_R_arr)
Irr_R=(5/1024)*Irr_R

Irr_R=(460*Irr_R)/(5-Irr_R)
c=3.3337*10**3
m=-0.4791

Irr=(Irr_R/c)**(1/m)
print("irradiance = ",Irr);


#plot the experimental I_V
#plt.figure(1)
#plt.plot(V,I)


#smoothing data
#window_size =2  # Adjust for desired smoothing level
#I_smooth = savgol_filter(I.values, window_size, 1)


#gaussian smoothing
sigma = .7
I_smooth =ndimage.gaussian_filter(I, sigma)

Iph=np.max(I_smooth)

#plotting the experimental and smoothened data
#plt.scatter(V,I)
#plt.plot(V,I_smooth)
I_exp=np.insert(I_smooth,0,Iph)
V_exp=np.insert(V,0,0)
y=Iph-I_exp
x=V_exp
#plot data after reversing
#plt.figure(2)
#plt.plot(x,y)

#finding the tolerance for fitting the curve
tol=8e-3
prev_err=1e20;
prev_tol=1e20;
for i in range(1,20):
    for j in range(9,0,-1):
        tol=j*1*10**(-i)
        y_log=np.log((y+tol))
        A=np.ones(len(x))
        A=A.reshape(-1,1)
        x_reshaped=x.reshape(-1,1)
        A=np.hstack((A,x_reshaped))
        An=np.dot(A.T,A)
        Bn=np.dot(A.T,y_log)
        xn = np.linalg.lstsq(An, Bn, rcond=None)[0]
        
        
        exp_term1=np.exp(xn[0])
        exp_term2=np.exp(xn[1]*x)
        y_fitpoints=exp_term1*exp_term2
        err_arr=np.abs(y_fitpoints-y)
        err=np.sum(err_arr)
        if (err>prev_err):
            break
        prev_tol=tol
        prev_err=err
    if (err>prev_err):
        break

#fitting the data by matrix
tol=prev_tol;

#tol=.00001
y_log=np.log(y+tol) 
A=np.ones(len(x))
A=A.reshape(-1,1)
x_reshaped=x.reshape(-1,1)
A=np.hstack((A,x_reshaped))
An=np.dot(A.T,A)
Bn=np.dot(A.T,y_log)
xn = np.linalg.lstsq(An, Bn, rcond=None)[0]           
xx=np.linspace(0,20,1000)
exp_term1=np.exp(xn[0])
exp_term2=np.exp(xn[1]*xx)
y1=exp_term1*exp_term2


#plotting the fitted data I_V
#plt.plot(xx,y1)

fig, ax1 = plt.subplots()
ax1.plot(xx, y1, color='b', label='current')
ax1.set_xlabel('Voltage(V)')
ax1.set_ylabel('current(A)')
ax1.tick_params(axis='y', labelcolor='b')
ax1.set_axis_on()
ax1.set_xlim(xmin=0)
ax1.set_ylim(ymin=0)
ax1.grid(True,which='both')
ax1.set_title("I_V curve")
ax1.scatter(x,y)
# Add a legend
#ax1.legend(loc='upper left')
plt.show()


#reversing the data
I_process_ini=-y1+Iph
I_processed=I_process_ini[I_process_ini>=0]
V_processed=xx[I_process_ini>=0]

P=I_processed*V_processed
P_max=np.max(P)
idx=np.argmax(P)
V_max=V_processed[idx]
I_max=I_processed[idx]
fig, ax1 = plt.subplots()
ax1.plot(V_processed, I_processed, color='b', label='current')
ax1.set_xlabel('Volatage(V)')
ax1.set_ylabel('current(A)', color='b')
ax1.tick_params(axis='y', labelcolor='b')
ax2 = ax1.twinx()
ax2.plot(V_processed, P, color='r', label='power',linestyle='dashed')
ax2.set_ylabel('power(watt)', color='r')
ax2.tick_params(axis='y', labelcolor='r')

# Add a legend
ax1.legend(loc='upper left')
ax2.legend(loc='upper right')
ax1.grid(True,which='both')
ax1.scatter(V, I,color='b' )
plt.title("solar panel characteristics")

plt.show()

#finding the series and parallel resistance
Isc=Iph
Voc=np.max(V_processed)
FF=P_max/(Voc*Isc)
h=V_processed[1]-V_processed[0]
conductance=(I_processed[1]-I_processed[0])/h
resistance=1/conductance
Rp=-resistance

#finding the efficiency
Area=0.31*0.34
P_in=Area*Irr
efficiency=P_max/P_in

#printing the necessary data
print("\n \n \n")
print("efficiency,                         =",efficiency)
print("fill factor,                        =",FF)
print("open circuit voltage,          Voc  =",Voc)
print("short circuit current,         Isc  =",Voc)
print("maximum power,                 Pmax =",P_max)
print("voltage at max power point,    Vmax =",V_max)
print("current at max power point,    Imax =",I_max)
print("parallel resistance,           Rp   =" ,Rp)
print("\n \n \n")
