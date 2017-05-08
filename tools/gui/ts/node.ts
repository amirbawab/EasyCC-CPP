class node {
    public static TYPE_INITIAL = "INITIAL";
    public static TYPE_NORMAL = "NORMAL";
    public static TYPE_FINAL = "FINAL";
    public id : number;
    public type : string;
    public token : string;
    public backtrack : boolean;
    public edges : edge[];

    public object() : any {
        return {
            id: this.id,
            label: this.id
        };
    }
}