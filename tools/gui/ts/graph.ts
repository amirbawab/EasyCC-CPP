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

        // Search for the edge
        for(let tmp of this.edges) {
            if(tmp.source == source && tmp.target == target) {
                return tmp;
            }
        }

        // If not found, create it
        let e = new edge();
        e.source = source;
        e.target = target;
        this.edges.push(e);
        return e;
    }

    public deleteNode(node : node) {
        // Delete node
        for(let i=0; i < this.nodes.length; i++) {
            if(this.nodes[i] == node) {
                this.nodes.splice(i,1);
                break;
            }
        }

        // Delete related edges
        let relatedEdges : edge[] = [];
        for(let i=0; i < this.edges.length; i++) {
            if(this.edges[i].source == node || this.edges[i].target == node) {
                relatedEdges.push(this.edges[i]);
            }
        }
        for(let e of relatedEdges) {
            this.deleteEdge(e.source, e.target);
        }
    }

    public deleteEdge(source : node, target : node) : boolean {
        for(let i=0; i < this.edges.length; i++) {
            let e : edge = this.edges[i];
            if(e.source == source && e.target == target) {
                this.edges.splice(i,1);
                return true;
            }
        }
        return false;
    }

    public deleteEdgeLabel(source : node, target : node, label : string) : boolean {
        for(let i=0; i < this.edges.length; i++) {
            let e : edge = this.edges[i];
            if(e.source == source && e.target == target) {
                let found : boolean = false;
                for(let j=0; j < e.label.length; j++) {
                    if(e.label[j] == label) {
                        e.label.splice(j,1);
                        found = true;
                        break;
                    }
                }
                if(e.label.length == 0) {
                    this.edges.splice(i,1);
                }
                console.log(found);
                return found;
            }
        }
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

    public reassignIds() {
        for(let i=0; i < this.nodes.length; i++) {
            this.nodes[i].id = i;
        }
        this.id = this.nodes.length;
    }

    public json() : any {
        let json = {};
        json["states"] = [];
        for(let n of this.nodes) {
            json["states"].push(n.json());
        }

        json["transitions"] = [];
        for(let e of this.edges) {
            json["transitions"].push(e.json());
        }
        return json;
    }

    public readJson(jsonStr : string) : boolean {
        try {
            let json = JSON.parse(jsonStr);

            // If keys not defined
            if(!json.hasOwnProperty("states") || !json.hasOwnProperty("transitions")) {
                return false;
            }

            // Create nodes
            for(let s of json.states) {
                let n = new node();
                n.id = s.id;
                n.type = s.type;
                if(n.type == node.TYPE_FINAL) {
                    n.backtrack = s.backtrack;
                    n.token = s.token;
                }
                this.nodes[n.id] = n;
            }

            // Update graph next id
            this.id = this.nodes.length;

            // Create edges
            for(let t of json.transitions) {
                let e = this.addEdge(this.nodes[t.from], this.nodes[t.to]);
                e.label = t.chars;
            }
        } catch (e) {
            return false;
        }
        return true;
    }
}