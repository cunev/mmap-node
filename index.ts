const bbmmap = require("./bbmmap.node");

export function mmap(handle: number, buffer: Buffer): boolean {
  return bbmmap.mmap(handle, buffer);
}
