import DAN

ServerIP = 'http://IP:9999' # or 'https://DomainName'
Comm_interval = 0.5 # unit:second

def profile_init():
    DAN.profile['dm_name']='DemoSet'
    DAN.profile['d_name']= 'Demo1' #DAN.profile['dm_name']+'.'+DAN.get_mac_addr()[-4:]

def odf():  # int only
    return [
        ('Switch-O1', 0, 'Switch-O1'),
        ('Switch-O2', 0, 'Switch-O2'),
        ('Switch-O3', 0, 'Switch-O3'),
    ]

def idf():
    return [
       ('AtPressure', float),
       ('Humidity', float),
       ('Temperature', float),
       ('UV', float),
       ('SoilMoisture', float),
       ('Bugs', int),
    ]
