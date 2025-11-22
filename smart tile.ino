import machine
import time
import urequests

# Telegram Bot Credentials
BOT_TOKEN = "6768943868:AAG1QRkv764uS5_lVCOC-KySrk"
CHAT_ID = "5313086585"
TELEGRAM_URL = f"https://api.telegram.org/bot{BOT_TOKEN}/sendMessage"

# Hardware Pins
PIEZO_PIN1 = 32  # Pin connected to Mat 1 (Piezo sensor)
BUZZER_PIN = 4   # Pin connected to Buzzer
SERVO_PIN = 2    # Pin connected to Servo motor

# Threshold for activation
THRESHOLD = 200  

# Initialize hardware
piezo_sensor = machine.ADC(machine.Pin(PIEZO_PIN1))
piezo_sensor.atten(machine.ADC.ATTN_11DB)  # Full range 0-3.3V

buzzer = machine.Pin(BUZZER_PIN, machine.Pin.OUT)
servo = machine.PWM(machine.Pin(SERVO_PIN), freq=50)

# Send Telegram Message
def send_telegram_message(message):
    payload = {"chat_id": CHAT_ID, "text": message}
    try:
        response = urequests.post(TELEGRAM_URL, json=payload)
        response.close()
    except Exception as e:
        print("Failed to send message:", e)

# Rotate Servo
def set_servo_angle(angle):
    duty = int((angle / 180) * 102 + 26)  # Convert angle to duty cycle
    servo.duty(duty)

# Main Loop
while True:
    piezo_value1 = piezo_sensor.read()  # Read piezo sensor value
    print(f"Mat 1: {piezo_value1}")

    if piezo_value1 > THRESHOLD:  # Pressure detected
        buzzer.on()
        set_servo_angle(180)
        send_telegram_message("Person detected on Mat 1. Servo rotated to 180°.")
    else:  # No pressure
        buzzer.off()
        set_servo_angle(0)

    time.sleep(0.5)
