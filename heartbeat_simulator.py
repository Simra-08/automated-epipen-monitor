import random
import math

t = 0
anomaly_mode = False
anomaly_counter = 0

def generate_heartbeat():
    global t, anomaly_mode, anomaly_counter

    t += 0.2

    # Smooth natural heartbeat variation
    base = 78 + 6 * math.sin(t)
    noise = random.uniform(-2, 2)
    heart_rate = base + noise

    # 5% chance to trigger anomaly
    if not anomaly_mode and random.random() < 0.05:
        anomaly_mode = True
        anomaly_counter = 0

    if anomaly_mode:
        heart_rate = random.choice([
            random.uniform(130, 150),  # High spike
            random.uniform(40, 50)     # Sudden drop
        ])
        anomaly_counter += 1

        if anomaly_counter > 5:
            anomaly_mode = False

    return int(heart_rate)

