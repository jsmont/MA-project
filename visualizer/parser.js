
function Graph(){
    this.rootNodes= [];
    this.nodeIndexes= {};
    this.nodes= [];
}

Graph.prototype.parse = function(text){
    this.rootNodes= [];
    this.nodeIndexes= {};
    this.nodes= [];

    var lines = text.split("\n");

    for(var i = 0; i < lines.length; ++i){
        var line = lines[i];
        this.parseLine(line);
    }
};

Graph.prototype.parseLine = function(line){
    var sides = line.split(":");
    var source = this.getNodeIndex(sides[0].trim());
    var destinations = sides[1].split(" ").map(String.trim).map(this.getNodeIndex);

    if(source == "0") this.rootNodes = Array.concat(this.rootNodes, destinations);
    else {
        this.nodes[source].destinations = Array.concat(this.nodes[src].destinations, destinations);
    }

    for( var i = 0; i < destinations.length; ++i) {
        this.nodes[destinations[i]].sources.push(source);
    }

}

Graph.prototype.getNodeIndex = function(id){

    if(typeof this.nodeIndexes[id] == "undefined"){
        this.nodeIndexes[id] = this.nodes.length;

        this.nodes.push({
            id: id,
            sources: [],
            destinations: [],
            type: "N/S"
        });
    }

    return this.nodeIndexes;
}
