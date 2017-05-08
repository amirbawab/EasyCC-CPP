class edge {
    public source : node;
    public target : node;
    public label : string[] = [];

    public addLabel(label : string) : boolean {
        if(this.label.indexOf(label) >= 0) {
            return false;
        }
        this.label.push(label);
        return true;
    }

    public object() : any {
        return {
            from: this.source.id,
            to: this.target.id,
            label: this.label.join(",")
        };
    }

    public json() : any {
        let json = {};
        json["from"] = this.source.id;
        json["to"] = this.target.id;
        json["chars"] = this.label;
        return json;
    }
}