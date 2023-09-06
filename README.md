#### `First off, apologies for not using doxygen/doxybook. I am still trying to get it to work...`

# **Task 6 Documentation**
### Task 6.1 IMU Yaw Angle:

-   This program communicates with the MPU-6050 IMU sensor.

-   Communication is done via I2C communication.

-   Connect the IMU sensor to the Arduino Uno similar to the following diagram, but `it is recommended to connect the sensor's vcc to 3.3v to avoid damaging the sensor.`
  
<img src="imgs\WhatsApp Image 2023-09-06 at 04.55.01.jpg" alt="First Image" width="" height="300">

-   After uploading the code to the Arduino wait for about 5 seconds till the board calibrates. When readings appear, you can move the sensor.

-   Readings of the angle about the Z-axis should start appearing.

-   If for some reason the program doesn't start reupload the code.

#### Answer to the Question:

-   First, we would need to use a `low pass filter` as the IMU communicates at lower frequencies than those causing the issue of the noise. Though if complexity is not an issue a better fit would be a Kalman filter as its filtration would be much more accurate.

-   Given the following info we can set the bandwith of the sensor as we see fit, but we could consider the "worst case scenario". Thus, if we assume the sensor communicates at 256Hz, then we could simply multiply said value by 120% to have a bit of a threshold. We would now end up with a cut-off frequency of `~307.2Hz`

<img src="imgs\Screenshot 2023-09-06 051052.png" alt="First Image" width="" height="600">


### Task 6.2 Motor Encoder:

-   This program senses pulses from the motor's encoder and increments/decrements a variable.

-   This variable can then be used along with the wheel's radius to estimate the distance the robot moved. For e.g. radius=0.2m, encoder has 540ppr, if we now calculate the circumference of the wheel we would get: `0.2\*PI\*2 = 1.257m`. Now we can divide the wheel's circ. by the ppr to find the distance between each pulse. This would give: `1.257/540 = 2.33e-3`. If we now multiply the counter of pulses from the code with the distance between each pulse we would approximately find the distance moved.

-   You could wire your system according to the following schematic...

<img src="imgs\pic.png" alt="First Image" width="" height="250">

-   A second order Butterworth low pass filter was used to filter the readings of the encoder. Using a second order rather than a first order may introduce some delay, but ultimately makes the readings more accurate.

#### Answer to the Question:

-   To find the max frequency of the signal from the motor encoder we can do some mathematical equations. First, calculate the circumference of the wheel: `0.2\*PI\*2 = 1.257m`. Second divide the circumference by ppr to find distance between each pulse: `1.257/540 = 2.33e-3`. Now we could divide the distance between each pulse by the wheel's speed to find the time between each pulse: `2.33e-3/0.5 = 4.66e-3`. Finally, we would get the reciprocal of the period between each pulse to get the frequency. This would leave us with a max frequency of `214.59Hz`. There is still a final step we need to make so that we could use the low pass filter, and that would be to multiply by 130% as to give the filter a bit of a threshold. This would leave us with a cut-off frequency of `279.318Hz`.

-   Alternatively, we could simply use the following equations to calculate the frequency:    

<img src="imgs\pic2.png" alt="First Image" width="" height="">
<img src="imgs\Screenshot 2023-09-06 205028.png" alt="First Image" width="" height="">




