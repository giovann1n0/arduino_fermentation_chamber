import serial
import pandas as pd
import time
from datetime import datetime

# Configure the serial port
# You may need to change the port name and baud rate to match your Arduino setup
ser = serial.Serial('COM3', 9600, timeout=1)

# Create an empty DataFrame to store the data
df = pd.DataFrame(columns=['Timestamp', 'Temperature'])

try:
    while True:
        # Read a line from the serial port
        line = ser.readline().decode('utf-8').strip()
        
        if line:
            # Parse the temperature value
            temperature = float(line)
            
            # Get the current timestamp
            timestamp = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
            
            # Append the new data to the DataFrame
            new_row = pd.DataFrame({'Timestamp': [timestamp], 'Temperature': [temperature]})
            df = pd.concat([df, new_row], ignore_index=True)
            
            print(f"Recorded: {timestamp} - Temperature: {temperature}°C")
            
            # Save the DataFrame to a CSV file every 10 records
            if len(df) % 10 == 0:
                df.to_csv('csv/temperature_data.csv', index=False)
                print("Data saved to CSV file")
        
        # Wait for a short time before reading the next value
        time.sleep(2)

except KeyboardInterrupt:
    print("Data collection stopped by user.")
    
finally:
    # Close the serial port and save any remaining data
    ser.close()
    df.to_csv('csv/temperature_data.csv', index=False)
    print("Final data saved to CSV file")