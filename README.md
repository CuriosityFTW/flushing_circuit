<h1 align="center">Flushing circuit for ventilator</h1>

**Company:** Noccarc Robotics Pvt. Ltd.

**Location:** Pune, India

**Duration:** Oct 2020 - Mar 2023

**Role:** Systems Engineer

---

***Project objective:*** *Development of a flushing circuit to clean ventilator sensors and pneumatic circuit*

**My contributions:**
While designing ventilators at Noccarc Robotics, my team encountered a critical challenge – contaminated moisture and fluids from patients' respiratory tracts deteriorate ventilator tubes and sensors over time, necessitating regular cleaning (or flushing) of these components, even during ventilator operation. Conventional flushing systems, characterized by bulky, high-pressure gas units as flushing sources, and lack of flow control mechanisms, result in gas leakage, inaccuracies in flow measurement, and selectively clean the components. To mitigate these deficiencies, I developed a novel method that utilized the ventilator’s mixing chamber (an oxygen-air mixture generator) as the flushing source – with the same pressure as the patient circuit, thereby eliminating the need for a high-pressure gas source. Further, I configured solenoid valves to regulate an oxygen-air mixture delivery to flush inspiratory and expiratory tubes, along with flow and pressure sensors, effectively preventing gas leakage and ensuring measurement accuracy. Using an STM32 microcontroller, I programmed a C++ control algorithm to operate the valves strategically, ensuring efficient flushing, and concurrently record sensors’ data for pressure, flow, and volume calculations for precise gas delivery to patients. This novel blend of hardware, control systems, and code enabled a compact, efficient solution to a confounding problem, for which I secured a patent.
