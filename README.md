## Digitally Adjustable Phrenic Nerve (DAPhNe) Stimulator System
### Northwestern University BME Senior Capstone Design (2016-2017)
#### Center for Autonomic Medicine in Pediatrics (CAMP) at Ann & Robert H. Lurie Children's Hospital of Chicago

<br></br>

Most recent documentation:


[**Digitally Adjustable Phrenic Nerve (DAPhNe) Stimulator - Getting Started, Rev 1.0**](https://github.com/AlexeyRevinski/DAPhNe-Stimulator-System/blob/master/Reference%20Documentation/DAPhNe_Getting_Started_v_1_0.pdf)


[**Digitally Adjustable Phrenic Nerve (DAPhNe) Stimulator - Technical Reference, Rev 1.1**](https://github.com/AlexeyRevinski/DAPhNe-Stimulator-System/blob/master/Reference%20Documentation/DAPhNe_Technical_Reference_v_1_1.pdf)

<br></br>

The focus of this project is the development and testing of a novel implantable phrenic nerve stimulator, to be used in patients with Congenital Central Hypoventilation Syndrome (CCHS) - specifically, children. The following sections summarize the project; to get to know the project in extensive detail, please refer to project documents under "Reference Documentation".

<br></br>

## Project Description
### Introduction to CCHS
Patients with CCHS have diminished autonomic nervous system function, which results in a variety of symptoms, the most prominent of which is insufficient ventilation. This results from poor phrenic nerve conduction; phrenic nerve is responsible for stimulating diaphragmatic contractions. While some patients are affected only mildly, some cannot breathe on their own, and require 24/7 ventilatory support.

### Current Solutions
This is usually done with a mechanical positive pressure ventilator; oxygenated air is forced into the patient's lungs by a mechanical piston. While this is a proven solution to the problem, these devices are enormous to carry around for small children; they are loud and extremely expensive. Patients also require a tracheotomy to be performed for this to work.

Another solution is to stimulate the phrenic nerve electrically. The only device that does this right now is the [**Mark IV stimulator**](http://www.averybiomedical.com/breathing-pacemakers/system-information/) from Avery Biomedical. It consists of an external transmitter that sends pulses inductively through tissue to a passive receiver that interfaces with an electrode in the vicinity of the nerve. This is the only solution on the market, and, while it has several advantages over the mechanical ventilator, it also has a variety of shortcomings that include:
 - Restricted mobility - antennae must be always attached to patient’s body for successful stimulation
 - System does not support multiple activity level settings, so each patient is forced to use two or three transmitters and mechanically switch between them each time the patient participates in a different activity (sleep vs. walk vs. run vs. eat, etc.)
 - Weight and size of the transmitter box - weight = 2 lb, volume = 511 cm3. A small child must carry this weight constantly
 - Unintuitive user interface - the device has non-linear, poorly labeled dials
 - Required presence of the external transmitter close to the patient's body restricts the patient's ability to participate in water-related activities.

### DAPhNe Stimulator System
The Digitally Adjustable Phrenic Nerve (DAPhNe) Stimulator System aims to lift those drawbacks and provide children with CCHS an implantable stimulator that would eliminate the need to be tethered to an external controller, allow for a variety of programmable settings to be switched "on the fly", and allow participation on water-related activities without fear of device malfunction or electric shock. 

The above objectives are accomplished by digitalization of the system, employing wireless charging, and wireless short range communication.  The DAPhNe stimulator is battery powered and charged inductively through skin; it provides constant stimulation to the nerve on its own without the need for an external inductive antenna. The external controller uses NFC technology to wirelessly transmit new activity level parameters to the implant, and the implant automatically adjusts its internal variables on the next breath. Thus, the patient is not tethered to anything, and the transition between various activity levels is seamless. Based on most recent power consumption testing and calculations, the device in its current state would be able to support the patient for one week on one battery charge, after which the battery is recharged wirelessly.

Currently, the DAPhNe Stimulator System consists of two custom-designed PCB prototype boards interfacing with some off-the-shelf components. This prototype was recently tested for equivalence against the Avery Biomedical Mark IV system in New Zealand white rabbits, and proved successful in stimulating the phrenic nerve effectively. The external user interface is being developed concurrently. 
<br></br>
<p align="center">
  <img src="https://github.com/AlexeyRevinski/BME390/blob/master/Visuals/Collage.png" width="750"/>
</p>
<p align="right">
  <img src="https://github.com/AlexeyRevinski/BME390/blob/master/Visuals/Overall%20Circuit%20Diagram-1.png" width="800"/>
</p>
