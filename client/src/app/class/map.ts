export class Map {
    id : Number;
    name: String;
    date: String

    constructor(id:Number, name:String, date: String ){
        this.id = id;
        this.name = name;
        this.date = date;

    }

}

export const UNSET_MAP_INDEX: number = -1;