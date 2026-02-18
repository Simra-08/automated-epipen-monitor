import requests

API_KEY = "WoD7QRNtUKyeIBHL38fpkl4AOcSb0C9uhX5gGMJZxazYF2ETsrrKc17bvUDqdVxQR3SigOYG94M6LhyT"
SENDER_ID = "FSTSMS"   # Default sender
ROUTE = "q"            # Quick route
TARGET_NUMBER = "917676367592"  # Without + sign

def send_alert(heart_rate):

    url = "https://www.fast2sms.com/dev/bulkV2"

    message = f"ALERT! Abnormal Heart Rate Detected: {heart_rate} BPM. Immediate attention required."

    payload = {
        "sender_id": SENDER_ID,
        "message": message,
        "route": ROUTE,
        "numbers": TARGET_NUMBER
    }

    headers = {
        "authorization": API_KEY
    }

    response = requests.post(url, data=payload, headers=headers)

    print("SMS Status:", response.text)
