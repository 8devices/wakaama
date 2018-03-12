const Emulate = require('restserver-api').Emulate;
const Client = Emulate.Client;
const RESOURCE_TYPE = Emulate.RESOURCE_TYPE;

let client_port = 5000;

class ClientInterface extends Client {

  constructor() {
    super(600, '8devices', '8dev_test', false, 'test', 'localhost', client_port++);

    this.createObject(3303, 0);
    this.objects['3303/0'].addResource(5700, 'R', RESOURCE_TYPE.FLOAT, 20.0);

    this.name = this.endpointClientName;
  }

  connect(addr, callback) {
    // TODO: set address

    this.on('state-change', state => {
      if (state === 'registered') {
        callback(null, {});
      }
    });

    this.start();
  }

  disconnect() {
    this.on('state-change', state => {
      if (state === 'stopped') {
        setTimeout(() => {
          this.coapServer.close();
        }, 100);
      }
    });

    this.stop();
  }

  set temperature(t) {
    this.objects['3303/0'].resources['5700'].value = t;
  }

}

module.exports = ClientInterface;
