{
  "targets": [{
    "target_name": "uhttp",
    "include_dirs": [
      "<!@(node -p \"require('node-addon-api').include\")",
      "uWebSockets/src"
    ],
    "dependencies": ["<!(node -p \"require('node-addon-api').gyp\")"],
    "sources": [
      "./lib/uhttp.cpp",
      "./lib/uws/Group.cpp",
      "./lib/uws/Networking.cpp",
      "./lib/uws/Hub.cpp",
      "./lib/uws/Node.cpp",
      "./lib/uws/WebSocket.cpp",
      "./lib/uws/HTTPSocket.cpp",
      "./lib/uws/Socket.cpp"
    ],
    "xcode_settings": {
      "OTHER_CFLAGS": [
        "-O3",
        "-std=c++11",
        "-D_GNU_SOURCE"
      ]
    },
    "defines": ["NAPI_DISABLE_CPP_EXCEPTIONS"],
    "conditions": [
      ['OS=="win"', {
        "link_settings": {
          "libraries": [
            "-lws2_32.lib"
          ]
        }
      }],
      ['OS=="linux"', {
        'cflags_cc': [ '-std=c++17', '-DUSE_LIBUV' ],
        'cflags_cc!': [ '-fno-exceptions', '-std=gnu++0x', '-fno-rtti' ],
        'cflags!': [ '-fno-omit-frame-pointer' ],
        'ldflags!': [ '-rdynamic' ],
        'ldflags': [ '-s' ]
      }]
    ],
  }]
}
