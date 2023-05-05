// index.js
// 获取应用实例
const app = getApp()

const { connect } = require('../../utils/mqtt')

const mqttHost = 'broker-cn.emqx.io'  //mqtt服务器域名
const mqttPort = '8084'   //mqtt服务器端口

const deviceSubTopic = '/mysmarthome/subs'  //设备订阅Topic (小程序发布命令的Topic)
const devicePubTopic = '/mysmarthome/pubs'  //设备发布Topic (小程序订阅数据的Topic)

const mpSubTopic = devicePubTopic
const mpPubTopic = deviceSubTopic

Page({
  data: {
    client: null,
    Temp:0,
    Hum:0,
    Light:0,
    LED:false,
    Beep:false
  },

  //LED发送函数
  onLedChange(event){
    const that = this
    //console.log(event,detail);
    console.log(event.detail.value);
    const sw = event.detail.value
    that.setData({Led:sw})

    if(sw){
      that.data.client.publish(mpPubTopic,JSON.stringify({
        target:"Led",
        value:1
      }),function(err){
        if(!err){
          console.log('成功下发指令-开灯');
        }
      })
    }else{
      that.data.client.publish(mpPubTopic,JSON.stringify({
        target:"Led",
        value:0
      }),function(err){
        if(!err){
          console.log('成功下发指令-关灯');
        }
      })
    }

  },

  //Beep发送函数
  onBeepChange(event){
    const that = this
    //console.log(event,detail);
    console.log(event.detail.value);
    const sw = event.detail.value
    that.setData({Led:sw})

    if(sw){
      that.data.client.publish(mpPubTopic,JSON.stringify({
        target:"Beep",
        value:1
      }),function(err){
        if(!err){
          console.log('成功下发指令-打开报警器');
        }
      })
    }else{
      that.data.client.publish(mpPubTopic,JSON.stringify({
        target:"Beep",
        value:0
      }),function(err){
        if(!err){
          console.log('成功下发指令-关闭报警器');
        }
      })
    }

  },

  // 事件处理函数
  onLoad() {
    const that = this
    that.setData({
      //wxs实际上就是wss => wss实际上就是拥有SSL加密通信的websockect协议
      client: connect(`wxs://${mqttHost}:${mqttPort}/mqtt`)   //连接的MQTT服务器域名和端口
    })
    that.data.client.on('connect',function(params){
      console.log('成功');
      wx.showToast({
        title: '连接成功',
        icon:'success',
        mask:'true'
      })
      that.data.client.subscribe(mpSubTopic,function(err){
        if(!err){
          console.log('成功订阅设备上行数据Topic!')
        }
      })
    })

    that.data.client.on('message',function(topic,message){
      console.log(topic);
      //console.log(message);//!?
      //message是16进制的buffer字节流

      let dataFromDev = {}
      try {
        dataFromDev = JSON.parse(message)
        console.log(dataFromDev);       
        that.setData({                          //接收数据
          Temp:dataFromDev.Temp,
          Hum:dataFromDev.Hum,
          Light:dataFromDev.Light,
          LED:dataFromDev.LED,
          Beep:dataFromDev.Beep,
        })
      } catch (error) {
        console.log('JSON解析失败',error);   
      }
    })

  },
})
