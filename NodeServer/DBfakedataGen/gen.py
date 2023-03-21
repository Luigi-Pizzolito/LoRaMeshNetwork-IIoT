import random
import datetime
import time
import json
from random import randrange

def generate_fake_data(start_date, end_date, time_interval, temp_range, humidity_range, soil_moisture_range):
    data = []
    current_date = start_date
    while current_date <= end_date:
        current_time = time.mktime(current_date.timetuple())
        id = randrange(4)
        temp = round(random.uniform(temp_range[0], temp_range[1]), 2)
        humidity = round(random.uniform(humidity_range[0], humidity_range[1]), 2)
        soil_moisture = round(random.uniform(soil_moisture_range[0], soil_moisture_range[1]), 2)
        
        # TODO: restructure this to meet the JSON structure defined in ../JSONformats/reply_format.json
        data.append({'id':id,'ts':current_time,'data':{'temperature': temp, 'humidity': humidity, 'soil_moisture': soil_moisture}})
        
        current_date += datetime.timedelta(minutes=time_interval)
    return data

# get current time and a week back
end_date = datetime.datetime.now()
start_date = end_date + datetime.timedelta(days=-7)
time_interval = 30 # in minutes
# set fakedata ranges
temp_range = (15, 30) # in Celsius
humidity_range = (40, 80) # in %
soil_moisture_range = (20, 60) # in %
# generate fake data
print('Generating Past Week of Data...')
print(start_date, "to", end_date)
fake_data = generate_fake_data(start_date, end_date, time_interval, temp_range, humidity_range, soil_moisture_range)
# write fake data to json file
print('Done, writing data.json')
f = open("data.json", "w")
f.write(json.dumps(fake_data))
f.close()
# print(fake_data)
print('Job Finished.')