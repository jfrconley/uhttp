import * as events from "events";
import {join} from "path";
import {RequestOptions} from "http";

const binding: any = require("node-gyp-build")(join(__dirname, ".."));

export interface ServerOptions {
    reusePort: boolean;
}

export class Server extends events.EventEmitter {
    private hub = Buffer.alloc(binding.sizeof_uhttp_server);

    constructor() {
        super();
        binding.init(this.hub, this, this.onConnection);
        binding.uhttp_on_fatal_exception(this.onFatal);
    }

    public listen(port: number, address: string = "0.0.0.0", callback: () => void) {
        binding.listen(this.hub, port, address);
    }

    private onConnection(req: any, res: any) {
        console.log((req.headerBuffer as Buffer).toString("base64"));
    }

    private onFatal(err: Error) {
        console.error(err);
    }

    private close() {
        binding.close(this.hub);
    }
}
// const {http} = require("uws");
//
// export class Server {
//     private server = http.createServer(this.handler);
//
//     public listen(port: 8080) {
//         this.server.listen(port);
//     }
//
//     private handler(req: any, res: any) {
//         console.log(req);
//         req.on("data", (stuff: any) => {
//             console.log(Buffer.from(stuff).toString("utf8"));
//            res.end();
//         });
//     }
// }
