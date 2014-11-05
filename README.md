LeapGesture-library
===================

This thesis studies the new possibilities to gesture interfaces that emerged with a Leap Motion sensor.
The Leap Motion is an innovative, 3D motion capturing device designed especially for hands and fingers tracking with precision up to 0.01mm.

The outcome of the thesis is the LeapGesture library dedicated to the developers for Leap Motion Controller that contains algorithms allowing to learn and recognize gestures.
The authors examined the data provided by the sensor in context of recognition of hand poses (static gestures), hand movements (dynamic gestures) and in task of a finger recognition.

The static gestures are recognized using the Support Vector Machine (SVM) with median filtering an input data and using the correspondences between consecutive recognitions.
The thesis contains evaluation of different feature sets, which have a significant impact on the recognition rate.
The chosen feature set allowed to recognize a set of five gestures with 99% accuracy and a set of ten gestures with 85%.

The dynamic gestures (movements of a hand and fingers) are recognized with the Hidden Markov Models (HMM). 
Recognition with HMMs allowed to achieve accuracy of 80% for a set containing six classes of dynamic gestures.
Finger recognition algorithms proposed in this thesis works with 93% accuracy on a recorded dataset.

The LeapGesture library contains presented approaches using a C++ interface, that can be easily used in any application for many purposes.
