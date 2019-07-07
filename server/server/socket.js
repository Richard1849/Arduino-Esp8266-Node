
var mongoose = require('mongoose');
let express = require('express');
let app = new express();
var mongo = require('../models/mongodb');
server = require('http').createServer(app);
let io = require('socket.io').listen(server);
var net = require("net");

// server.listen(3001);
//服务器监听所有客户端，并返回该新连接对象
mongoose.connect('mongodb://47.102.102.23:27017/test');
   mongoose.connection.on("connected", function() {
     console.log('连接数据库成功')
  })
   mongoose.connection.on("error", function() {
     console.log('连接数据库不成功')
   })
   mongoose.connection.on("disconnect", function() {
     console.log('断开数据库连接')
   })
   io.sockets.on('connection', function (ioData) {
    var datas = {}
    setInterval(() => {
      mongo.find({}, (err,doc) => {
        datas = doc[doc.length-1]
        ioData.emit('name', datas);
      })
    }, 2000)
    ioData.emit('name', {username: 'litingting ' + new Date()});
    ioData.on('my other event', function (data) {
      console.log(data)
      if(data.buttonState === 'open') {
        mongo.update({state: 1}, (err, doc) => {
          if(err) console.log(err)
          console.log('保存成功：' + doc)
        })
      }
      if(data.buttonState === 'close') {
        mongo.update({state: 0}, (err, doc) => {
          if(err) console.log(err)
          console.log('保存成功：' + doc)
        })
      }
    })
  })
 server.listen(3000, function(){
   console.log("Creat socket server on http://127.0.0.1:3000/");
 })