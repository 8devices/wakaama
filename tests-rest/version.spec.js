const chai = require('chai');
const chai_http = require('chai-http');
const server = require('./server-if');
const { exec } = require('child_process');

const should = chai.should();
chai.use(chai_http);

describe('Version', function () {
  before(function (done) {
    server.start();

    done();
  });

  after(function (done) {
    done();
  });

  describe('GET /version', function() {

    it('should return 200 and correct version', function(done) {
      chai.request(server)
        .get('/version')
        .end(function (err, res) {
          should.not.exist(err);

          exec('git describe --abbrev=0 --tags', (err, stdout, stderr) => {
            const version = stdout.replace('\n','');
            res.text.should.deep.equal(version);
          });

          res.should.have.status(200);

          done();
        });
    });
  });
});
