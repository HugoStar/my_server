from flask_restful import Resource, reqparse
from flask_jwt_extended import jwt_required, get_jwt, get_jwt_identity
from models.item import ItemModel


class Item(Resource):
    parser = reqparse.RequestParser()

    parser.add_argument('is_light',
                    type=bool,
                    required=True,
                    help="This field cannot be left blank!"
                    )

    parser.add_argument('humidity_up',
                type=bool,
                required=True,
                help="This field cannot be left blank!"
                )

    parser.add_argument('humidity_down',
                    type=bool,
                    required=True,
                    help="This field cannot be left blank!"
                    )

    parser.add_argument('humidity',
                        type=float,
                        required=True,
                        help="This field cannot be left blank!"
                        )

    parser.add_argument('temperature',
                        type=float,
                        required=True,
                        help="This field cannot be left blank!"
                        )

    parser.add_argument('light_on',
                        type=float,
                        required=True,
                        help="This field cannot be left blank!"
                        )

    parser.add_argument('light_off',
                        type=float,
                        required=True,
                        help="This field cannot be left blank!"
                        )

    parser.add_argument('prepare_humidity',
                        type=float,
                        required=True,
                        help="This field cannot be left blank!"
                        )

    parser.add_argument('current_light',
                        type=float,
                        required=True,
                        help="This field cannot be left blank!"
                        )

    parser.add_argument('tdd',
                        type=float,
                        required=True,
                        help="This field cannot be left blank!"
                        )

    def get(self, name):
        item = ItemModel.find_by_name(name)
        if item:
            return item.json()
        return {'message': 'Item not found'}, 404

    def post(self, name):
        data = Item.parser.parse_args()
        if ItemModel.find_by_name(name):
            item = ItemModel.find_by_name(name)
            item.update(**data)
            item.save_to_db()
            return item.json()
        data = Item.parser.parse_args()
        item = ItemModel(name, **data)
        try:
            item.save_to_db()
        except:
            return {"message": "An error occurred inserting the item."}, 500

        return item.json(), 201
 
    def delete(self, name):
        item = ItemModel.find_by_name(name)
        if item:
            item.delete_from_db()
            return {'message': 'Item deleted.'}
        return {'message': 'Item not found.'}, 404

class ItemList(Resource):
    def get(self):
        items = [elem.json() for elem in ItemModel.find_all()]
        return {'items': items}, 200
