from db import db


class ItemModel(db.Model):
    __tablename__ = 'items'

    id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.String(80))
    is_light = db.Column(db.Integer)
    humidity_up = db.Column(db.Integer)
    humidity_down = db.Column(db.Integer)
    humidity = db.Column(db.Float)
    temperature = db.Column(db.Float)
    light_on = db.Column(db.Float)
    light_off = db.Column(db.Float)
    prepare_humidity = db.Column(db.Float)
    current_light = db.Column(db.Float)
    tdd = db.Column(db.Float)

    def __init__(self, name, is_light, humidity_up, humidity_down, humidity, temperature, light_on, light_off, prepare_humidity, current_light, tdd):
        self.name = name
        self.is_light = is_light
        self.humidity_up = humidity_up
        self.humidity_down = humidity_down
        self.humidity = humidity
        self.temperature = temperature
        self.light_on = light_on
        self.light_off = light_off
        self.prepare_humidity = prepare_humidity
        self.current_light = current_light
        self.tdd = tdd

    def update(self, is_light, humidity_up, humidity_down, humidity, temperature, light_on, light_off, prepare_humidity, current_light, tdd):
        self.is_light = is_light
        self.humidity_up = humidity_up
        self.humidity_down = humidity_down
        self.humidity = humidity
        self.temperature = temperature
        self.light_on = light_on
        self.light_off = light_off
        self.prepare_humidity = prepare_humidity
        self.current_light = current_light
        self.tdd = tdd

    def json(self):
        return {
            'id': self.id, 
            'name': self.name,
            'is_light': self.is_light,
            'humidity_up': self.humidity_up, 
            'humidity_down': self.humidity_down,  
            'humidity': self.humidity,
            'temperature': self.temperature,
            'light_on': self.light_on,
            'light_off': self.light_off,
            'prepare_humidity': self.prepare_humidity,
            'current_light': self.current_light,
            'tdd': self.tdd,
            }

    @classmethod
    def find_by_name(cls, name):
        return cls.query.filter_by(name=name).first()

    @classmethod
    def find_all(cls):
        return cls.query.all()

    def save_to_db(self):
        db.session.add(self)
        db.session.commit()

    def delete_from_db(self):
        db.session.delete(self)
        db.session.commit()
