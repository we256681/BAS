class FingerprintDataClass {
	constructor(OnData)
	{
		this.DataPending = [];
		this.Key = "";
		this.OnData = OnData
		this.LastSent = 0
		var self = this
		setInterval(function(){
			self.AddData("")
		},1000)
	}

	SetKey(Key)
	{
		this.Key = Key
		this.AddData("")
	}

	AddData(NewDataString)
	{
		var NewDataList = NewDataString.split(",")
		if(NewDataString.length > 0)
			this.DataPending = this.DataPending.concat(NewDataList)
		let now = Date.now()
		if(!this.Key || (this.LastSent != 0 && now - this.LastSent < 5000) || this.DataPending.length == 0)
		{
			return
		}

		this.LastSent = now;
		let SendData = {
			headers: {
				'Accept': 'application/json',
				'Content-Type': 'application/json'
			},
			method: "POST",
			body: JSON.stringify(this.DataPending.slice(0,500))
		}
		this.DataPending = this.DataPending.slice(500)
		let self = this

		fetch(`https://fingerprints.bablosoft.com/detector/decrypt?key=${this.Key}`, SendData)
          .then(function(response) {
            return response.json()
          }).then(function(json) {
          	if(json["valid"])
				self.OnData(json["data"].map(function(x){var res = JSON.parse(x); res["type"] = "fingerprint"; return res}))
          })

	}

}