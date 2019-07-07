var mongoose = require('mongoose')
var Schema = mongoose.Schema
var dataSchema = new Schema({
  "id": String,
  wet: Number,
  temp:Number,
  state: Number
})

module.exports = mongoose.model('Data', dataSchema)