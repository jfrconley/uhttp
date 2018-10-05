const {join} = require("path");

const {Server} = require("node-gyp-build")(join(__dirname, "."));

const server = new Server((data) => {
    console.log("yay");
    console.log(data);
});
server.listen(8080);