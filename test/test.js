const bbmmap = require('./bbmmap.node')

const handle = bbmmap.openProcess(31016);

bbmmap.mmap(handle, require('fs').readFileSync('./discordoverlay_dx11.dll'), 1)
console.log(bbmmap);