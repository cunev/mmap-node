const bbmmap = require("./bbmmap.node");

export function mmap(handle: number, buffer: Buffer): boolean {
  return bbmmap.mmap(handle, buffer);
}

export function openHandle(windowName: string): number {
  return bbmmap.openProcess(windowName);
}

export function closeHandle(handle: number): void {
  bbmmap.closeHandle(handle);
}
