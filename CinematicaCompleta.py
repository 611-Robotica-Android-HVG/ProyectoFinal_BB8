import serial
import math
import time
from coppeliasim_zmqremoteapi_client import RemoteAPIClient


# SERIAL
arduino = serial.Serial('COM13', 115200)
time.sleep(2)

client = RemoteAPIClient()
sim = client.getObject('sim')

left_wheel = sim.getObject('/leftWheel')
right_wheel = sim.getObject('/rightWheel')

servo1_joint = sim.getObject('/servo1')
servo2_joint = sim.getObject('/servo2')


# PARÁMETROS ROBOT
L = 0.0605   # distancia entre ruedas
R = 0.0147   # radio rueda

# brazo
L1 = 0.094   # longitud brazo
h = 0.064    # altura base


# CINEMÁTICA DIFERENCIAL
def diff_kinematics(v, w):
    wl = (v - (L / 2) * w) / R
    wr = (v + (L / 2) * w) / R
    return wl, wr


sim.startSimulation()
print("Sistema iniciado...")

while True:
    try:

        line = arduino.readline().decode(errors='ignore').strip()
        v, w, s1, s2 = map(float, line.split(","))


        # RUEDAS
        wl, wr = diff_kinematics(v, w)

        sim.setJointTargetVelocity(left_wheel, wl)
        sim.setJointTargetVelocity(right_wheel, wr)

        # SERVOS (grados → rad)
        s1_rad = math.radians(s1)
        s2_rad = math.radians(s2)

        sim.setJointTargetPosition(servo1_joint, s1_rad)
        sim.setJointTargetPosition(servo2_joint, s2_rad)


        # CINEMÁTICA BRAZO (DIRECTA)
        theta1 = s1_rad
        theta2 = s2_rad

        # proyección del segundo eslabón
        r = L1 * math.cos(theta2)
        z = h + L1 * math.sin(theta2)

        # posición en plano del brazo
        x_local = r * math.cos(theta1)
        y_local = r * math.sin(theta1)
        


        print(f"v={v:.2f} w={w:.2f} | s1={s1:.1f} s2={s2:.1f}")
        print(f"EE -> x={x_local:.3f} y={y_local:.3f} z={z:.3f}")
        print(f"Wheel velocities -> left: {wl:.2f} rad/s | right: {wr:.2f} rad/s")
        print("------------------------------")

    except Exception as e:
        print("Error:", e)

        sim.setJointTargetVelocity(left_wheel, 0)
        sim.setJointTargetVelocity(right_wheel, 0)
