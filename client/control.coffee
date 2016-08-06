#
# Remote control via socket connection

program = require('commander')
net = require('net')

# default values
cip = null
cport = 2000

# program parameters
program
  .version('0.0.1')
  .arguments('<ip> [port]')
  .action (ip, port) ->
    cip = ip
    if port?
      cport = port


class WifiCarControl
  constructor: (@server, @port) ->
    @connected = false

  connect: () ->
    self = @
    console.log "Connecting to %s:%d", @server, @port
    client = net.connect {host: @server, port: @port}, () ->
      console.log "Connected to server"
      self.connected = true

    client.on 'data', (data) ->
      console.log(data.toString())

    client.on 'end', () ->
      console.log 'disconnected from server'
      self.connected = false

main = () ->
  program.parse(process.argv)
  if cip == null
    console.error "server ip must be given"
    process.exit(1)

  wcc = new WifiCarControl(cip, cport)
  wcc.connect()

if require.main == module
  main()