class edge {
    public source : node;
    public target : node;
    public label : string;

    public object() : any {
        return {
            from: this.source.id,
            to: this.target.id,
            label: this.label
        };
    }
}