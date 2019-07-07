
var mongoose = require('mongoose');
let express = require('express');
let app = new express();
var mongo = require('../models/mongodb');
// server = require('http').createServer(app);
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
/* 创建TCP服务器 */
var server = net.createServer(function(socket){
   console.log('someone connects');
    // socket.write('server_data')
   /* 监听data事件 */
 socket.on('data',function(data){
 console.log(JSON.parse(data.toString()));
 var newData = JSON.parse(data.toString())
 mongo.create(newData, function(err, doc) {
   if(err) console.log(err);
  })
 })
 setInterval(() => {
  var id = '5d06014ff6ab186a3b1c6068'
  mongo.findById(id, function(err, doc) {
   if(err) console.log(err);
   if(doc.state === 0) {
    socket.write('closeLight')
   } else if(doc.state === 1){
    socket.write('openLight')
   }
  })
 }, 1000)
 })
/* 设置连接的服务器 */
 server.listen(3001, function(){
   console.log("Creat TCP server on http://127.0.0.1:3001/");
 })