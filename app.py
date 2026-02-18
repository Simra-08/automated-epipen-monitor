from flask import Flask, render_template, jsonify
from heartbeat_simulator import generate_heartbeat
from sms_alert import send_alert
import threading
import time

app = Flask(__name__)

current_heartbeat = 80
alert_sent = False

def monitor_heartbeat():
    global current_heartbeat, alert_sent

    while True:
        current_heartbeat = generate_heartbeat()

        # Anomaly detection
        if current_heartbeat < 55 or current_heartbeat > 120:
            if not alert_sent:
                send_alert(current_heartbeat)
                alert_sent = True
        else:
            alert_sent = False

        time.sleep(2)

@app.route("/")
def dashboard():
    return render_template("dashboard.html")

@app.route("/heartbeat")
def heartbeat():
    return jsonify({"heartbeat": current_heartbeat})

if __name__ == "__main__":
    threading.Thread(target=monitor_heartbeat).start()
    app.run(debug=True)
