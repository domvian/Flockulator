# Flockulator
A granular synth that uses face tracking the flocking algorithm to control its parameters.
(Makeshift) Face tracking data from max MSP is sent over via OSc to wekinator where a model has been trained on different facial expressions.
Different facial expressions will alter the parameters used for the flocking algorithm, which is sent again via OSC from wekinator to 
OpenFrameworks. The parameters for the granualtor are taken from data such as the speed, distance and XY positions of the boids in the 
OpenFrameworks application. 




REQUIREMENTS 

- Max MSP 
- Wekinator
- Openframeworks



How to run:

1. Download jit.cv and place folder in your Max MSP packages folder. 

2. Open faceOSC.maxpat and make sure webcam is working 

3. Open Wekinator and open Grainmotion.wekproj that can be found in the Grainmotion folder

4. Make sure that you are receiving OSC data in Wekinator and that the slider values are changing with the webcam

5. Run Openframeworks application (found in the bin folder within the Flockulator folder)  and indulge in the experience (make sure to leave Wekinator and Max MSP running in the background) 

