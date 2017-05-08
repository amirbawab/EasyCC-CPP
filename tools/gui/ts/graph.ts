class graph {
    private id : number = 0;
    public nodes : node[] = [];
    public edges : edge[] = [];

    public addNode() : node {
        let n = new node();
        n.id = this.id++;
        this.nodes.push(n);
        return n;
    }

    public addEdge(source : node, target : node) : edge {
        let e = new edge();
        e.source = source;
        e.target = target;
        this.edges.push(e);
        return e;
    }

    public nodesObjects() : any {
        let nodesObjects : any[] = [];
        for(let n of this.nodes) {
            nodesObjects.push(n.object());
        }
        return nodesObjects;
    }

    public edgesObjects() : any {
        let edgesObjects : any[] = [];
        for(let e of this.edges) {
            edgesObjects.push(e.object());
        }
        return edgesObjects;
    }
}