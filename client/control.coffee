#
# Remote control via socket connection

program = require('commander')

cip = null
cport = 2000

program
  .version('0.0.1')
  .arguments('<ip> [port]')
  .action (ip, port) ->
    cip = ip
    if port?
      cport = port

program.parse(process.argv)
if cip == null
  console.error "server ip must be given"
  process.exit(1)

console.log "Connecting to %s:%d", cip, cport
