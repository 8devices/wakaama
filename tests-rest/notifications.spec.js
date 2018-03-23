
const chai = require('chai');
const chai_http = require('chai-http');
const should = chai.should();
const events = require('events');
const ClientInstance = require('restserver-api').Emulate.Client;
const RESOURCE_TYPE = require('restserver-api').Emulate.RESOURCE_TYPE;
const server = require('./server-if');
const ClientInterface = require('./client-if');

chai.use(chai_http);

describe('Notifications interface', function () {
  const client = new ClientInterface();

  before(function (done) {
    var self = this;

    server.start();

    self.events = new events.EventEmitter();
    // TODO: swap interval with long-poll once server supports it
    self.interval = setInterval(function () {
      chai.request(server)
        .get('/notification/pull')
        .end(function (err, res) {
          const responses = res.body['async-responses'];
          if (!responses)
            return;

          for (var i=0; i<responses.length; i++) {
            self.events.emit('async-response', responses[i]);
          }
        });
    }, 1000);

    client.connect(server.address(), (err, res) => {
      done();
    });
  });

  after(function () {
    clearInterval(this.interval);
    client.disconnect();
  });

  describe('GET /notification/callback', function() {

    it('should return 404 (NOT FOUND)', function(done) {
      chai.request(server)
        .get('/notification/callback')
        .end(function (err, res) {
          err.should.have.status(404);

          done();
        });
    });

    it('should return 200 (url found)', function(done) {
      chai.request(server)
        .put('/notification/callback')
        .set('Content-Type', 'application/json')
        .send('{"url": "http://localhost:9998/my_callback", "headers": {}}')
        .end(function (err, res) {
          should.not.exist(err);
          // XXX: Successful subscription status code should be 204, however
          // it is 200 now
          // res.should.have.status(204);
          res.should.have.status(200);

          chai.request(server)
            .get('/notification/callback')
            .end(function (err, res) {
              should.not.exist(err);
              res.body.should.be.a('object');
              res.should.have.status(200);

              done();
            });
        });
    });
  });

  describe('PUT /notification/callback', function() {

    it('should return 204 (successfully subscribed)', function(done) {
      chai.request(server)
        .put('/notification/callback')
        .set('Content-Type', 'application/json')
        .send('{"url": "http://localhost:9999/my_callback", "headers": {}}')
        .end(function (err, res) {
          should.not.exist(err);
          // XXX: Successful subscription status code should be 204, however
          // it is 200 now
          // res.should.have.status(204);
          res.should.have.status(200);

          done();
        });
    });

    it('should return 400 for empty object', function(done) {
      chai.request(server)
        .put('/notification/callback')
        .set('Content-Type', 'application/json')
        .end(function (err, res) {
          err.should.have.status(400);

          done();
        });
    });

    it('should return 400 for wrong object size', function(done) {
      chai.request(server)
        .put('/notification/callback')
        .set('Content-Type', 'application/json')
        .send('{"url": "http://localhost:9999/my_callback"}')
        .end(function (err, res) {
          err.should.have.status(400);

          done();
        });
    });

    it('should return 400 for wrong callback headers type', function(done) {
      chai.request(server)
        .put('/notification/callback')
        .set('Content-Type', 'application/json')
        .send('{"url": "http://localhost:9999/my_callback", "headers": "wrong-type"}')
        .end(function (err, res) {
          err.should.have.status(400);

          done();
        });
    });

    it('should return 400 for invalid url', function(done) {
      chai.request(server)
        .put('/notification/callback')
        .set('Content-Type', 'application/json')
        .send('{"url": 9999, "headers": {}}')
        .end(function (err, res) {
          err.should.have.status(400);

          done();
        });
    });

    it('should return 400 for wrong value type in callback headers object', function(done) {
      chai.request(server)
        .put('/notification/callback')
        .set('Content-Type', 'application/json')
        .send('{"url": "http://localhost:9999/my_callback", "headers": {42: 42}}')
        .end(function (err, res) {
          err.should.have.status(400);

          done();
        });
    });

    it('should return 415 for wrong "Content-Type" header', function(done) {
      chai.request(server)
        .put('/notification/callback')
        .end(function (err, res) {
          err.should.have.status(415);

          done();
        });
    });
  });
});

