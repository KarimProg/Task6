# **Task 6 Documentation**
### Task 6.1 IMU Yaw Angle:

-   This program communicates with the MPU-6050 IMU sensor.

-   Communication is done via I2C communication.

-   Connect the IMU sensor to the Arduino Uno similar to the following diagram, but `it is recommended to connect the vcc to 3.3v to avoid damaging the board.`
  
<img src="imgs\WhatsApp Image 2023-09-06 at 04.55.01.jpg" alt="First Image" width="" height="300">

-   After uploading the code to the Arduino wait for about 5 seconds till the board calibrates. When readings appear, you can move the sensor.

-   Readings of the angle about the Z-axis should start appearing.

-   If for some reason the program doesn't start reupload the code.

#### Answer to Question:

-   First, we would need to use a `low pass filter` as the IMU communicates at lower frequencies than those causing the issue of the noise. Though if complexity is not an issue a better fit would be a Kalman filter as its filtration would be much more accurate.

-   Given the following info we can set the bandwith of the sensor as we see fit, but we could consider the "worst case scenario". Thus, if we assume the sensor communicates at 256Hz, then we could simply multiply said value by 120% to have a bit of a threshold. We would now end up with a cut-off frequency of `~307.2Hz`

<img src="imgs\Screenshot 2023-09-06 051052.png" alt="First Image" width="" height="600">


### Task 6.2 Motor Encoder:

-   