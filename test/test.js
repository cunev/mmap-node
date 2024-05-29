const bbmmap = require('../bbmmap.node')

const handle = bbmmap.openProcess("osu!");

bbmmap.mmap(handle, require('fs').readFileSync('./discordoverlay_dx11.dll'))
console.log(bbmmap);

setInterval(() => {}, );