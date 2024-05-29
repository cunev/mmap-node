{
  "targets": [
    {
      "target_name": "bbmmap",
      "sources": [
        "src/main.cc"
      ],
      'dependencies': ["<!(node -p \"require('node-addon-api').gyp\")"],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "./lib/include"
      ],
      "conditions": [
        ['OS=="win"', {
          "msvs_settings": {
            "VCCLCompilerTool": {
              "PreprocessorDefinitions": [
                "_WINDOWS"
              ],
              "ExceptionHandling": 1
            },
            "VCLinkerTool": {
              "AdditionalDependencies": [
                "../lib/blackbone.lib",
                "../lib/diaguids.lib",
                "../lib/MemoryModule.lib",
              ],
            }
          }
        }]
      ]
    }
  ]
}