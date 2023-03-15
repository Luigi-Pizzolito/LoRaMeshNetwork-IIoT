import random
import datetime
import json

def generate_fake_data(start_date, end_date, time_interval, temp_range, humidity_range, soil_moisture_range):
    data = {}
    current_date = start_date
    while current_date <= end_date:
        current_time = current_date.strftime("%Y-%m-%d %H:%M:%S")
        temp = round(random.uniform(temp_range[0], temp_range[1]), 2)
        humidity = round(random.uniform(humidity_range[0], humidity_range[1]), 2)
        soil_moisture = round(random.uniform(soil_moisture_range[0], soil_moisture_range[1]), 2)
        data[current_time] = {'temperature': temp, 'humidity': humidity, 'soil_moisture': soil_moisture}
        current_date += datetime.timedelta(minutes=time_interval)
    return data

# Example usage
# start_date = datetime.datetime(2023, 3, 15, 0, 0, 0)
# end_date = datetime.datetime(2023, 3, 16, 0, 0, 0)
end_date = datetime.datetime.now()
start_date = end_date + datetime.timedelta(days=-7)
time_interval = 30 # in minutes
temp_range = (15, 30) # in Celsius
humidity_range = (40, 80) # in %
soil_moisture_range = (20, 60) # in %


print('Generating Past Week of Data...')
print(start_date, "to", end_date)

fake_data = generate_fake_data(start_date, end_date, time_interval, temp_range, humidity_range, soil_moisture_range)

print('Done, writing data.json')

f = open("data.json", "w")
f.write(json.dumps(fake_data))
f.close()

# print(fake_data)
print('Job Finished.')