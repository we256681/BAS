_InMail.findConfig = function(domain, protocol){
	var configs = this.configs();
	var domainObj = configs[domain];
	if(!domainObj){
		for(var key in configs){
			var obj = configs[key];
			if(obj.domains && obj.domains.indexOf(domain) > -1){
				domainObj = obj;
				break;
			};
		};
	};
	
	if(domainObj && domainObj[protocol]){
		return domainObj[protocol];
	};
	
	return null;
};
_InMail.configs = function(){
	return {
		"mail.com": {
			imap: {
				host: "imap.mail.com",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "pop.mail.com",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			},
			domains: ["mail.org","email.com","post.com","usa.com","accountant.com","consultant.com","dr.com","engineer.com","cheerful.com","techie.com","linuxmail.org","europe.com","london.com","uymail.com","myself.com","iname.com","writeme.com","2die4.com","activist.com","adexec.com","africamail.com","aircraftmail.com","alabama.usa.com","alaska.usa.com","allergist.com","alumni.com","alumnidirector.com","americamail.com","amorous.com","angelic.com","archaeologist.com","arizona.usa.com","artlover.com","asia-mail.com","atheist.com","australiamail.com","bartender.net","berlin.com","bigger.com","bikerider.com","birdlover.com","blader.com","boardermail.com","brazilmail.com","brew-master.com","california.usa.com","californiamail.com","caress.com","catlover.com","chef.net","chemist.com","chinamail.com","clerk.com","cliffhanger.com","collector.org","columnist.com","comic.com","computer4u.com","contractor.net","coolsite.net","counsellor.com","count.com","couple.com","cutey.com","cyberdude.com","cybergal.com","cyber-wizard.com","dallasmail.com","dbzmail.com","deliveryman.com","diplomats.com","disciples.com","doctor.com","doglover.com","doramail.com","dublin.com","earthling.net","elvisfan.com","englandmail.com","europemail.com","execs.com","fan.com","feelings.com","financier.com","fireman.net","florida.usa.com","footballer.com","gardener.com","geologist.com","germanymail.com","graduate.org","graphic-designer.com","hackermail.com","hairdresser.net","hilarious.com","hockeymail.com","homemail.com","hot-shot.com","hour.com","humanoid.net","illinois.usa.com","innocent.com","inorbit.com","instruction.com","instructor.net","insurer.com","irelandmail.com","italymail.com","japan.com","journalist.com","keromail.com","kittymail.com","koreamail.com","lawyer.com","legislator.com","loveable.com","lovecat.com","mad.scientist.com","madonnafan.com","madrid.com","marchmail.com","mexicomail.com","mindless.com","minister.com","mobsters.com","monarchy.com","moscowmail.com","munich.com","musician.org","muslim.com","newyork.usa.com","null.net","nycmail.com","oath.com","optician.com","pacificwest.com","petlover.com","photographer.net","playful.com","poetic.com","politician.com","popstar.com","presidency.com","priest.com","programmer.net","publicist.com","realtyagent.com","reborn.com","reggaefan.com","religious.com","repairman.com","representative.com","rescueteam.com","revenue.com","rocketship.com","rockfan.com","rome.com","royal.net","saintly.com","salesperson.net","sanfranmail.com","scientist.com","scotlandmail.com","secretary.net","seductive.com","singapore.com","snakebite.com","songwriter.net","soon.com","spainmail.com","teachers.org","technologist.com","texas.usa.com","thegame.com","therapist.net","toke.com","tokyo.com","toothfairy.com","tvstar.com","umpire.com","wallet.com","webname.com","weirdness.com","who.net","whoever.com","winning.com","witty.com","worker.com","workmail.com","yours.com"]
		},
		"mail.telenor.dk": {
			imap: {
				host: "mail.telenor.dk",
				port: 143,
				encrypt: "starttls",
				username: "%email%"
			},
			pop3: {
				host: "mail.telenor.dk",
				port: 110,
				encrypt: "starttls",
				username: "%email%"
			},
			domains: ["vip.cybercity.dk","post.cybercity.dk","email.dk","mobil.dk","privatmail.dk","info.dk","io.dk","it.dk","film.dk","worldonline.dk","wol.dk","1031.inord.dk","123mail.dk","12fuel.dk","12mail.dk","12move.dk","2senior.dk","anarki.dk","anderledes.dk","begavet.dk","bitnisse.dk","city.dk","cool.dk","cyberdude.dk","cyberjunkie.dk","dk2net.dk","dk-online.dk","elinstallatoer.dk","elsker.dk","elvis.dk","fald.dk","fedt.dk","feminin.dk","forening.dk","gadefejer.dk","gason.dk","grin.dk","grov.dk","hardworking.dk","heaven.dk","hemmelig.dk","huleboer.dk","image.dk","inbound.dk","indbakke.dk","infile.dk","jyde.dk","klog.dk","knus.dk","krudt.dk","kulturel.dk","larsen.dk","lazy.dk","lystig.dk","mail.dia.dk","maskulin.dk","min-postkasse.dk","musling.dk","natteliv.dk","netbruger.dk","pedal.dk","pengemand.dk","pokerface.dk","post.dia.dk","postman.dk","privat.dia.dk","quake.dk","ready.dk","secret.dk","sleepy.dk","sporty.dk","superbruger.dk","talent.dk","tanke.dk","taxidriver.dk","teens.dk","teknik.dk","tjekket.dk","traceroute.dk","tv.dk","ugenstilbud.dk","ungdom.dk","video.dk","vittig.dk"]
		},
		"126.com": {
			imap: {
				host: "imap.126.com",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			}
		},
		"163.com": {
			imap: {
				host: "imap.163.com",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			}
		},
		"a1.net": {
			imap: {
				host: "securemail.a1.net",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			domains: ["aon.at"]
		},
		"abc.plala.or.jp": {
			pop3: {
				host: "abc.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"active24.com": {
			imap: {
				host: "email.active24.com",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "email.active24.com",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			},
			domains: ["smtp.cz"]
		},
		"agate.plala.or.jp": {
			pop3: {
				host: "agate.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"aol.com": {
			imap: {
				host: "imap.aol.com",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "pop.aol.com",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			},
			domains: ["aim.com","netscape.net","netscape.com","compuserve.com","cs.com","wmconnect.com","aol.de","aol.it","aol.fr","aol.es","aol.se","aol.co.uk","aol.co.nz","aol.com.au","aol.com.ar","aol.com.br","aol.com.mx","mail.gm0.yahoodns.net"]
		},
		"alice.it": {
			imap: {
				host: "in.alice.it",
				port: 143,
				encrypt: "none",
				username: "%email%"
			},
			pop3: {
				host: "in.alice.it",
				port: 110,
				encrypt: "none",
				username: "%email%"
			}
		},
		"yahoo.com": {
			imap: {
				host: "imap.mail.yahoo.com",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "pop.mail.yahoo.com",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			},
			domains: ["yahoo.de","yahoo.it","yahoo.fr","yahoo.es","yahoo.se","yahoo.co.uk","yahoo.co.nz","yahoo.com.au","yahoo.com.ar","yahoo.com.br","yahoo.com.mx","ymail.com","rocketmail.com","mail.am0.yahoodns.net","am0.yahoodns.net","yahoodns.net"]
		},
		"amail.plala.or.jp": {
			pop3: {
				host: "amail.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"amber.plala.or.jp": {
			pop3: {
				host: "amber.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"att.net": {
			pop3: {
				host: "inbound.att.net",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			},
			domains: ["ameritech.net","bellsouth.net","flash.net","nvbell.net","pacbell.net","prodigy.net","sbcglobal.net","snet.net","swbell.net","wans.net"]
		},
		"broba.cc": {
			pop3: {
				host: "mail.broba.cc",
				port: 110,
				encrypt: "none",
				username: "%email%"
			},
			domains: ["garnet.broba.cc","amethyst.broba.cc","coral.broba.ccv","diamond.broba.cc"]
		},
		"onet.pl": {
			pop3: {
				host: "pop3.poczta.onet.pl",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			},
			domains: ["onet.eu","poczta.onet.pl","poczta.onet.eu","op.pl","vp.pl","autograf.pl","buziaczek.pl","amorki.pl","republika.pl"]
		},
		"apost.plala.or.jp": {
			pop3: {
				host: "apost.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"aqua.plala.or.jp": {
			pop3: {
				host: "aqua.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"arcor.de": {
			imap: {
				host: "imap.arcor.de",
				port: 993,
				encrypt: "ssl",
				username: "%login%"
			},
			pop3: {
				host: "pop3.arcor.de",
				port: 995,
				encrypt: "ssl",
				username: "%login%"
			}
		},
		"arubapec.it": {
			imap: {
				host: "imaps.pec.aruba.it",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "pop3s.pec.aruba.it",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			domains: ["pec.it","mypec.eu","gigapec.it","ingpec.eu"]
		},
		"ballade.plala.or.jp": {
			pop3: {
				host: "ballade.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"bay.wind.ne.jp": {
			pop3: {
				host: "bay.wind.ne.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			},
			domains: ["bay.wind.jp","bay.wind.co.jp","bay.gunmanet.or.jp","bay.gunmanet.ne.jp"]
		},
		"bb-niigata.jp": {
			pop3: {
				host: "pop.bb-niigata.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"dd.iij4u.or.jp": {
			pop3: {
				host: "mbox.iij4u.or.jp",
				port: 110,
				encrypt: "starttls",
				username: "%login%.%domain%"
			},
			domains: ["ff.iij4u.or.jp","hh.iij4u.or.jp","kk.iij4u.or.jp","nn.iij4u.or.jp","pp.iij4u.or.jp","rr.iij4u.or.jp","ss.iij4u.or.jp","bc.iij4u.or.jp","bk.iij4u.or.jp","bp.iij4u.or.jp","bu.iij4u.or.jp"]
		},
		"beige.plala.or.jp": {
			pop3: {
				host: "beige.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"skynet.be": {
			imap: {
				host: "imap.proximus.be",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "pop.proximus.be",
				port: 485,
				encrypt: "ssl",
				username: "%email%"
			},
			domains: ["proximus.be","belgacom.net","kidcity.be"]
		},
		"bell.net": {
			imap: {
				host: "imap.bell.net",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "pophm.sympatico.ca",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			},
			domains: ["sympatico.ca"]
		},
		"biglobe.ne.jp": {
			pop3: {
				host: "mail.biglobe.ne.jp",
				port: 110,
				encrypt: "none",
				username: "%email%"
			}
		},
		"bigpond.com": {
			imap: {
				host: "imap.telstra.com",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "pop.telstra.com",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			},
			domains: ["bigpond.net.au","telstra.com","bigpond.net"]
		},
		"mail.ru": {
			imap: {
				host: "imap.mail.ru",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "pop.mail.ru",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			},
			domains: ["inbox.ru","list.ru","bk.ru","corp.mail.ru"]
		},
		"libero.it": {
			imap: {
				host: "imapmail.libero.it",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "popmail.libero.it",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			},
			domains: ["iol.it","blu.it","inwind.it","giallo.it"]
		},
		"blue.plala.or.jp": {
			pop3: {
				host: "blue.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"swisscom.ch": {
			imap: {
				host: "imaps.bluewin.ch",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "pop3s.bluewin.ch",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			},
			domains: ["bluewin.ch","bluemail.ch"]
		},
		"blueyonder.co.uk": {
			imap: {
				host: "imap4.blueyonder.co.uk",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "pop3.blueyonder.co.uk",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			}
		},
		"bmail.plala.or.jp": {
			pop3: {
				host: "bmail.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"bol.com.br": {
			imap: {
				host: "imap.bol.com.br",
				port: 993,
				encrypt: "ssl",
				username: "%login%"
			},
			pop3: {
				host: "pop3.bol.com.br",
				port: 995,
				encrypt: "ssl",
				username: "%login%"
			}
		},
		"bolero.plala.or.jp": {
			pop3: {
				host: "bolero.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"bpost.plala.or.jp": {
			pop3: {
				host: "bpost.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"brown.plala.or.jp": {
			pop3: {
				host: "brown.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"btinternet.com": {
			imap: {
				host: "mail.btinternet.com",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "mail.btinternet.com",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			},
			domains: ["btopenworld.com","talk21.com"]
		},
		"camel.plala.or.jp": {
			pop3: {
				host: "camel.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"cameo.plala.or.jp": {
			pop3: {
				host: "cameo.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"ziggo.nl": {
			imap: {
				host: "imap.ziggo.nl",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "pop.ziggo.nl",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			},
			domains: ["hahah.nl","ziggomail.com","casema.nl","zinders.nl","zeggis.nl","zeggis.com","razcall.nl","razcall.com","upcmail.nl","chello.nl","multiweb.nl","home.nl","quicknet.nl"]
		},
		"cc9.ne.jp": {
			pop3: {
				host: "pop.cc9.ne.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"cek.ne.jp": {
			pop3: {
				host: "mail.cek.ne.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"centurylink.net": {
			imap: {
				host: "mail.centurylink.net",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "pop.centurylink.net",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			},
			domains: ["embarqmail.com"]
		},
		"centurytel.net": {
			pop3: {
				host: "pop.centurytel.net",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			},
			domains: ["clds.net","coastalnow.net","cochill.net","cswnet.com","emadisonriver.com","emadisonriver.net","gallatinriver.net","grics.net","gulftel.com","madisonriver.biz","mebtel.net"]
		},
		"cgl.ucsf.edu": {
			imap: {
				host: "plato.cgl.ucsf.edu",
				port: 993,
				encrypt: "ssl",
				username: "%login%"
			}
		},
		"charter.com": {
			imap: {
				host: "mobile.charter.net",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			domains: ["charter.net"]
		},
		"clio.ne.jp": {
			pop3: {
				host: "mail.clio.ne.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"sfr.fr": {
			imap: {
				host: "imap.sfr.fr",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "pop.sfr.fr",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			},
			domains: ["neuf.fr","club-internet.fr"]
		},
		"clustermail.de": {
			imap: {
				host: "mail.clustermail.de",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "mail.clustermail.de",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			}
		},
		"cmail.plala.or.jp": {
			pop3: {
				host: "cmail.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"versatel.de": {
			imap: {
				host: "mx.versatel.de",
				port: 143,
				encrypt: "none",
				username: "%email%"
			},
			pop3: {
				host: "mx.versatel.de",
				port: 143,
				encrypt: "none",
				username: "%email%"
			},
			domains: ["versanet.de","foni.net","gelsennet.de","telebel.de","telelev.de","cneweb.de","ruhrnet-online.de"]
		},
		"co1.wind.ne.jp": {
			pop3: {
				host: "co1.wind.ne.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			},
			domains: ["co1.wind.jp"]
		},
		"co2.wind.ne.jp": {
			pop3: {
				host: "co2.wind.ne.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			},
			domains: ["co2.wind.jp"]
		},
		"co3.wind.ne.jp": {
			pop3: {
				host: "co3.wind.ne.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			},
			domains: ["co3.wind.jp"]
		},
		"cocoa.plala.or.jp": {
			pop3: {
				host: "cocoa.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"coda.plala.or.jp": {
			pop3: {
				host: "coda.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"comcast.net": {
			imap: {
				host: "imap.comcast.net",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "mail.comcast.net",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			}
		},
		"concerto.plala.or.jp": {
			pop3: {
				host: "concerto.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"coral.plala.or.jp": {
			pop3: {
				host: "coral.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"courante.plala.or.jp": {
			pop3: {
				host: "courante.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"cox.net": {
			imap: {
				host: "imap.cox.net",
				port: 993,
				encrypt: "ssl",
				username: "%login%"
			},
			pop3: {
				host: "pop.cox.net",
				port: 995,
				encrypt: "ssl",
				username: "%login%"
			}
		},
		"cpost.plala.or.jp": {
			pop3: {
				host: "cpost.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"cream.plala.or.jp": {
			pop3: {
				host: "cream.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"cty-net.ne.jp": {
			pop3: {
				host: "pops.cty-net.ne.jp",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			},
			domains: ["m2.cty-net.ne.jp","m3.cty-net.ne.jp","m4.cty-net.ne.jp","m5.cty-net.ne.jp","m6.cty-net.ne.jp","m7.cty-net.ne.jp","m8.cty-net.ne.jp","m9.cty-net.ne.jp","cty-net.com"]
		},
		"dan.wind.ne.jp": {
			pop3: {
				host: "dan.wind.ne.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			},
			domains: ["dan.wind.jp","dan.wind.co.jp","dan.gunmanet.or.jp","dan.gunmanet.ne.jp"]
		},
		"dance.plala.or.jp": {
			pop3: {
				host: "dance.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"dmail.plala.or.jp": {
			pop3: {
				host: "dmail.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"dondominio.com": {
			imap: {
				host: "imap.dondominio.com",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "pop3.dondominio.com",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			}
		},
		"iijmio-mail.jp": {
			pop3: {
				host: "mbox.iijmio-mail.jp",
				port: 110,
				encrypt: "starttls",
				username: "%login%.%domain%"
			},
			domains: ["miomio.jp","miobox.jp","e23.jp","x-il.jp"]
		},
		"earthlink.net": {
			imap: {
				host: "imap.earthlink.net",
				port: 143,
				encrypt: "none",
				username: "%email%"
			},
			pop3: {
				host: "pop.earthlink.net",
				port: 110,
				encrypt: "none",
				username: "%email%"
			},
			domains: ["mindspring.com","ix.netcom.com"]
		},
		"ebony.plala.or.jp": {
			pop3: {
				host: "ebony.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"elpasotel.net": {
			imap: {
				host: "mail.elpasotel.net",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "pop3.elpasotel.net",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			}
		},
		"seznam.cz": {
			imap: {
				host: "imap.seznam.cz",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "pop3.seznam.cz",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			},
			domains: ["email.cz","post.cz","spoluzaci.cz"]
		},
		"email.it": {
			imap: {
				host: "imapmail.email.it",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "popmail.email.it",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			}
		},
		"email.plala.or.jp": {
			pop3: {
				host: "email.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"emailsrvr.com": {
			imap: {
				host: "secure.emailsrvr.com",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			}
		},
		"ewe.net": {
			pop3: {
				host: "pop.ewe.net",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			},
			domains: ["ewetel.de","teleos-web.de"]
		},
		"fantasy.plala.or.jp": {
			pop3: {
				host: "fantasy.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"fastwebnet.it": {
			imap: {
				host: "imap.fastwebnet.it",
				port: 143,
				encrypt: "starttls",
				username: "%email%"
			}
		},
		"flamenco.plala.or.jp": {
			pop3: {
				host: "flamenco.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"fmail.plala.or.jp": {
			pop3: {
				host: "fmail.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"free.fr": {
			imap: {
				host: "imap.free.fr",
				port: 993,
				encrypt: "ssl",
				username: "%login%"
			},
			pop3: {
				host: "pop.free.fr",
				port: 995,
				encrypt: "ssl",
				username: "%login%"
			}
		},
		"freenet.de": {
			imap: {
				host: "mx.freenet.de",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "mx.freenet.de",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			}
		},
		"fuga.plala.or.jp": {
			pop3: {
				host: "fuga.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"gandi.net": {
			imap: {
				host: "mail.gandi.net",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "mail.gandi.net",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			}
		},
		"gigahost.dk": {
			imap: {
				host: "mail.gigahost.dk",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "mail.gigahost.dk",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			}
		},
		"googlemail.com": {
			imap: {
				host: "imap.gmail.com",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "pop.gmail.com",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			},
			domains: ["gmail.com","google.com","jazztel.es"]
		},
		"gmail.plala.or.jp": {
			pop3: {
				host: "gmail.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"gmx.net": {
			imap: {
				host: "imap.gmx.net",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "pop.gmx.net",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			},
			domains: ["gmx.de","gmx.at","gmx.ch","gmx.eu","gmx.biz","gmx.org","gmx.info"]
		},
		"gmx.com": {
			imap: {
				host: "imap.gmx.com",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "pop.gmx.com",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			},
			domains: ["gmx.tm","gmx.us","gmx.co.uk","gmx.es","gmx.fr","gmx.ca","gmx.cn","gmx.co.in","gmx.com.br","gmx.com.my","gmx.hk","gmx.ie","gmx.ph","gmx.pt","gmx.ru","gmx.se","gmx.sg","gmx.tw","gmx.com.tr","gmx.it","gmx.li"]
		},
		"go.tvm.ne.jp": {
			pop3: {
				host: "go.tvm.ne.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"o2.pl": {
			pop3: {
				host: "poczta.o2.pl",
				port: 995,
				encrypt: "ssl",
				username: "%login%"
			},
			domains: ["go2.pl","tlen.pl","prokonto.pl"]
		},
		"1und1.de": {
			imap: {
				host: "imap.1und1.de",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "pop.1und1.de",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			},
			domains: ["online.de","onlinehome.de","sofortstart.de","sofort-start.de","sofortsurf.de","sofort-surf.de","go4more.de","kundenserver.de","schlund.de"]
		},
		"goneo.de": {
			imap: {
				host: "imap.goneo.de",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "pop.goneo.de",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			}
		},
		"goo.jp": {
			pop3: {
				host: "pop.mail.goo.ne.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"gransy.com": {
			imap: {
				host: "imap.gransy.com",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "imap.gransy.com",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			},
			domains: ["internetserver.cz"]
		},
		"grape.plala.or.jp": {
			pop3: {
				host: "grape.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"gray.plala.or.jp": {
			pop3: {
				host: "gray.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"hal.ne.jp": {
			pop3: {
				host: "mail.hal.ne.jp",
				port: 110,
				encrypt: "none",
				username: "%email%"
			}
		},
		"hana.or.jp": {
			pop3: {
				host: "mail.hana.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"hotmail.com": {
			imap: {
				host: "outlook.office365.com",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "outlook.office365.com",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			},
			domains: ["live.com","msn.com","windowslive.com","outlook.at","outlook.be","outlook.cl","outlook.cz","outlook.de","outlook.dk","outlook.es","outlook.fr","outlook.hu","outlook.ie","outlook.in","outlook.it","outlook.jp","outlook.kr","outlook.lv","outlook.my","outlook.ph","outlook.pt","outlook.sa","outlook.sg","outlook.sk","outlook.co.id","outlook.co.il","outlook.co.th","outlook.com.ar","outlook.com.au","outlook.com.br","outlook.com.gr","outlook.com.tr","outlook.com.vn","hotmail.be","hotmail.ca","hotmail.cl","hotmail.cz","hotmail.de","hotmail.dk","hotmail.es","hotmail.fi","hotmail.fr","hotmail.gr","hotmail.hu","hotmail.it","hotmail.lt","hotmail.lv","hotmail.my","hotmail.nl","hotmail.no","hotmail.ph","hotmail.rs","hotmail.se","hotmail.sg","hotmail.sk","hotmail.co.id","hotmail.co.il","hotmail.co.in","hotmail.co.jp","hotmail.co.kr","hotmail.co.th","hotmail.co.uk","hotmail.co.za","hotmail.com.ar","hotmail.com.au","hotmail.com.br","hotmail.com.hk","hotmail.com.tr","hotmail.com.tw","hotmail.com.vn","live.at","live.be","live.ca","live.cl","live.cn","live.de","live.dk","live.fi","live.fr","live.hk","live.ie","live.in","live.it","live.jp","live.nl","live.no","live.ru","live.se","live.co.jp","live.co.kr","live.co.uk","live.co.za","live.com.ar","live.com.au","live.com.mx","live.com.my","live.com.ph","live.com.pt","live.com.sg","livemail.tw","olc.protection.outlook.com"]
		},
		"i.softbank.jp": {
			imap: {
				host: "imap.softbank.jp",
				port: 993,
				encrypt: "ssl",
				username: "%login%"
			}
		},
		"ic-net.or.jp": {
			pop3: {
				host: "mail.ic-net.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"me.com": {
			imap: {
				host: "imap.mail.me.com",
				port: 993,
				encrypt: "ssl",
				username: "%login%"
			},
			domains: ["mac.com","icloud.com"]
		},
		"iiyama-catv.ne.jp": {
			pop3: {
				host: "mail.iiyama-catv.ne.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"imail.plala.or.jp": {
			pop3: {
				host: "imail.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"inbox.lv": {
			imap: {
				host: "mail.%domain%",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "mail.%domain%",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			},
			domains: ["inbox.eu","inbox.lt","inbox.la"]
		},
		"indigo.plala.or.jp": {
			pop3: {
				host: "indigo.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"inet-shibata.or.jp": {
			pop3: {
				host: "po.inet-shibata.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"internode.on.net": {
			imap: {
				host: "mail.internode.on.net",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "mail.internode.on.net",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			}
		},
		"ipax.at": {
			imap: {
				host: "mail.ipax.at",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			}
		},
		"df.eu": {
			imap: {
				host: "sslin.df.eu",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "sslin.df.eu",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			},
			domains: ["ispgateway.de"]
		},
		"ivory.plala.or.jp": {
			pop3: {
				host: "ivory.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"iwafune.ne.jp": {
			pop3: {
				host: "po.iwafune.ne.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"jade.plala.or.jp": {
			pop3: {
				host: "jade.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"janis.or.jp": {
			pop3: {
				host: "mail.%domain%",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"jet.ne.jp": {
			imap: {
				host: "imap.jet.ne.jp",
				port: 993,
				encrypt: "ssl",
				username: "%login%"
			},
			pop3: {
				host: "pop.jet.ne.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"ji.jet.ne.jp": {
			pop3: {
				host: "pop02.jet.ne.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"jmail.plala.or.jp": {
			pop3: {
				host: "jmail.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"kl.wind.ne.jp": {
			pop3: {
				host: "kl.wind.ne.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			},
			domains: ["kl.wind.jp","kl.wind.co.jp","kl.gunmanet.or.jp","kl.gunmanet.ne.jp","k1.wind.ne.jp","k1.wind.jp","k1.gunmanet.or.jp","k1.gunmanet.ne.jp"]
		},
		"kabelmail.de": {
			pop3: {
				host: "pop3.kabelmail.de",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			}
		},
		"kelcom.net": {
			pop3: {
				host: "pop1.kelcom.net",
				port: 110,
				encrypt: "none",
				username: "%email%"
			}
		},
		"khaki.plala.or.jp": {
			pop3: {
				host: "khaki.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"kmail.plala.or.jp": {
			pop3: {
				host: "kmail.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"kokuyou.ne.jp": {
			pop3: {
				host: "mail.kokuyou.ne.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"lapis.plala.or.jp": {
			pop3: {
				host: "lapis.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"laposte.net": {
			imap: {
				host: "imap.laposte.net",
				port: 993,
				encrypt: "ssl",
				username: "%login%"
			},
			pop3: {
				host: "pop.laposte.net",
				port: 995,
				encrypt: "ssl",
				username: "%login%"
			}
		},
		"lemon.plala.or.jp": {
			pop3: {
				host: "lemon.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"lilac.plala.or.jp": {
			pop3: {
				host: "lilac.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"lime.plala.or.jp": {
			pop3: {
				host: "lime.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"mx1.tiki.ne.jp": {
			pop3: {
				host: "%domain%",
				port: 110,
				encrypt: "none",
				username: "%login%"
			},
			domains: ["mx2.et.tiki.ne.jp","mx2.tiki.ne.jp","mx2.wt.tiki.ne.jp","mx3.et.tiki.ne.jp","mx3.tiki.ne.jp","mx4.et.tiki.ne.jp","mx4.tiki.ne.jp","mx5.et.tiki.ne.jp","mx5.tiki.ne.jp","mx6.et.tiki.ne.jp","mx6.tiki.ne.jp","mx7.tiki.ne.jp","mx8.tiki.ne.jp","mx9.tiki.ne.jp","mx21.tiki.ne.jp","mx22.tiki.ne.jp","mx31.tiki.ne.jp","mx32.tiki.ne.jp","mx35.tiki.ne.jp","mx36.tiki.ne.jp","mx41.tiki.ne.jp","mx51.et.tiki.ne.jp","mx51.tiki.ne.jp","mx52.tiki.ne.jp","mx61.tiki.ne.jp","mx71.tiki.ne.jp","mx81.tiki.ne.jp","mx82.tiki.ne.jp","mx91.tiki.ne.jp","ma100.tiki.ne.jp"]
		},
		"t-online.de": {
			imap: {
				host: "secureimap.t-online.de",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "securepop.t-online.de",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			},
			domains: ["magenta.de"]
		},
		"mahoroba.ne.jp": {
			pop3: {
				host: "mail.mahoroba.ne.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"mail.dk": {
			pop3: {
				host: "pop3.mail.dk",
				port: 110,
				encrypt: "none",
				username: "%email%"
			}
		},
		"mail.ee": {
			imap: {
				host: "%domain%",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "%domain%",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			}
		},
		"mail.wind.ne.jp": {
			pop3: {
				host: "mail.wind.ne.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			},
			domains: ["mail.wind.jp","mail.wind.co.jp","mail.gunmanet.or.jp","mail.gunmanet.ne.jp","mail.gunmanet.jp"]
		},
		"mail.iwafune.ne.jp": {
			pop3: {
				host: "mail.iwafune.ne.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"office365.com": {
			imap: {
				host: "outlook.office365.com",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "outlook.office365.com",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			},
			domains: ["onmicrosoft.com","mail.protection.outlook.com"]
		},
		"maroon.plala.or.jp": {
			pop3: {
				host: "maroon.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"sapo.pt": {
			imap: {
				host: "imap.sapo.pt",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "pop.sapo.pt",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			},
			domains: ["sapo.cv","sapo.ao","sapo.mz","sapo.tl","meo.pt"]
		},
		"MessagingEngine": {
			imap: {
				host: "imap.fastmail.com",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "pop.fastmail.com",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			},
			domains: ["messagingengine.com"]
		},
		"minuet.plala.or.jp": {
			pop3: {
				host: "minuet.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"ml.murakami.ne.jp": {
			pop3: {
				host: "ml.murakami.ne.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"pop.shibata.ne.jp": {
			pop3: {
				host: "%domain%",
				port: 110,
				encrypt: "none",
				username: "%login%"
			},
			domains: ["ml.shibata.ne.jp"]
		},
		"mnet.ne.jp": {
			pop3: {
				host: "mail.mnet.ne.jp",
				port: 110,
				encrypt: "none",
				username: "%email%"
			}
		},
		"mopera.net": {
			imap: {
				host: "mail.mopera.net",
				port: 993,
				encrypt: "ssl",
				username: "%login%"
			},
			pop3: {
				host: "mail.mopera.net",
				port: 995,
				encrypt: "ssl",
				username: "%login%"
			}
		},
		"yandex.ru": {
			imap: {
				host: "imap.yandex.com",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "pop.yandex.com",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			},
			domains: ["yandex.com","yandex.net","yandex.by","yandex.kz","yandex.ua","ya.ru","narod.ru"]
		},
		"navy.plala.or.jp": {
			pop3: {
				host: "navy.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"nifty.com": {
			pop3: {
				host: "pop.nifty.com",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"nsat.jp": {
			pop3: {
				host: "mail.nsat.jp",
				port: 110,
				encrypt: "none",
				username: "%email%"
			}
		},
		"ntlworld.com": {
			imap: {
				host: "imap.ntlworld.com",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "pop3.ntlworld.com",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			}
		},
		"ocn.ne.jp": {
			pop3: {
				host: "pop.ocn.ne.jp",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			},
			domains: ["ocn.ad.jp"]
		},
		"olive.plala.or.jp": {
			pop3: {
				host: "olive.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"one.com": {
			imap: {
				host: "imap.one.com",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			}
		},
		"one.cz": {
			imap: {
				host: "imap.registrator.cz",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "pop3.registrator.cz",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			}
		},
		"opal.plala.or.jp": {
			pop3: {
				host: "opal.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"orange.fr": {
			imap: {
				host: "imap.orange.fr",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "pop.orange.fr",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			},
			domains: ["wanadoo.fr"]
		},
		"orange.plala.or.jp": {
			pop3: {
				host: "orange.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"orchid.plala.or.jp": {
			pop3: {
				host: "orchid.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"outlook.com": {
			imap: {
				host: "outlook.office365.com",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "outlook.office365.com",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			}
		},
		"ovh.net": {
			imap: {
				host: "ssl0.ovh.net",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "ssl0.ovh.net",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			}
		},
		"pal.kijimadaira.jp": {
			pop3: {
				host: "mail.pal.kijimadaira.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"palette.plala.or.jp": {
			pop3: {
				host: "palette.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"parabox.or.jp": {
			pop3: {
				host: "pop3.parabox.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"pdx.edu": {
			imap: {
				host: "psumail.pdx.edu",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			}
		},
		"peach.plala.or.jp": {
			pop3: {
				host: "peach.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"peoplepc.com": {
			imap: {
				host: "imap.peoplepc.com",
				port: 143,
				encrypt: "none",
				username: "%email%"
			},
			pop3: {
				host: "pop.peoplepc.com",
				port: 110,
				encrypt: "none",
				username: "%email%"
			}
		},
		"plum.plala.or.jp": {
			pop3: {
				host: "plum.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"po.dcn.ne.jp": {
			pop3: {
				host: "po.dcn.ne.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"po.wind.ne.jp": {
			pop3: {
				host: "po.wind.ne.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			},
			domains: ["po.wind.jp","po.wind.co.jp","po.gunmanet.or.jp","po.gunmanet.ne.jp"]
		},
		"pobox.com": {
			imap: {
				host: "mail.pobox.com",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "mail.pobox.com",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			}
		},
		"polka.plala.or.jp": {
			pop3: {
				host: "polka.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"posteo.de": {
			imap: {
				host: "posteo.de",
				port: 143,
				encrypt: "starttls",
				username: "%email%"
			},
			domains: ["posteo.at","posteo.ch","posteo.org","posteo.eu"]
		},
		"ptd.net": {
			imap: {
				host: "promail.ptd.net",
				port: 993,
				encrypt: "ssl",
				username: "%login%"
			},
			pop3: {
				host: "promail.ptd.net",
				port: 995,
				encrypt: "ssl",
				username: "%login%"
			}
		},
		"purple.plala.or.jp": {
			pop3: {
				host: "purple.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"q.com": {
			imap: {
				host: "mail.q.com",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "mail.q.com",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			}
		},
		"qq.com": {
			imap: {
				host: "imap.qq.com",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			}
		},
		"rainbow.plala.or.jp": {
			pop3: {
				host: "rainbow.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"rambler.ru": {
			imap: {
				host: "imap.rambler.ru",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "pop.rambler.ru",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			},
			domains: ["rambler.ua", "autorambler.ru", "myrambler.ru", "lenta.ru", "ro.ru"]
		},
		"red.plala.or.jp": {
			pop3: {
				host: "red.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"rmail.plala.or.jp": {
			pop3: {
				host: "rmail.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"rondo.plala.or.jp": {
			pop3: {
				host: "rondo.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"rose.plala.or.jp": {
			pop3: {
				host: "rose.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"rouge.plala.or.jp": {
			pop3: {
				host: "rouge.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"rr.com": {
			imap: {
				host: "mail.twc.com",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "mail.twc.com",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			}
		},
		"ruby.plala.or.jp": {
			pop3: {
				host: "ruby.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"runestone.net": {
			pop3: {
				host: "mail.runestone.net",
				port: 995,
				encrypt: "ssl",
				username: "%login%"
			}
		},
		"strato.de": {
			imap: {
				host: "imap.strato.de",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "pop3.strato.de",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			},
			domains: ["rzone.de"]
		},
		"sakunet.ne.jp": {
			pop3: {
				host: "mail.sakunet.ne.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"sea.plala.or.jp": {
			pop3: {
				host: "sea.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"wildwestdomains.com": {
			imap: {
				host: "imap.secureserver.net",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "pop.secureserver.net",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			},
			domains: ["secureserver.net"]
		},
		"sepia.plala.or.jp": {
			pop3: {
				host: "sepia.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"serenade.plala.or.jp": {
			pop3: {
				host: "serenade.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"silk.plala.or.jp": {
			pop3: {
				host: "silk.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"silver.plala.or.jp": {
			pop3: {
				host: "silver.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"sky.com": {
			imap: {
				host: "imap.tools.sky.com",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "pop.tools.sky.com",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			}
		},
		"sky.plala.or.jp": {
			pop3: {
				host: "sky.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"smail.plala.or.jp": {
			pop3: {
				host: "smail.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"snow.plala.or.jp": {
			pop3: {
				host: "snow.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"so.wind.ne.jp": {
			imap: {
				host: "so.wind.ne.jp",
				port: 143,
				encrypt: "none",
				username: "%login%"
			},
			domains: ["so.wind.jp"]
		},
		"sonata.plala.or.jp": {
			pop3: {
				host: "sonata.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"studenti.univr.it": {
			imap: {
				host: "univr.mail.cineca.it",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "univr.mail.cineca.it",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			}
		},
		"suite.plala.or.jp": {
			pop3: {
				host: "suite.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"symphony.plala.or.jp": {
			pop3: {
				host: "symphony.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"taupe.plala.or.jp": {
			pop3: {
				host: "taupe.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"terra.es": {
			imap: {
				host: "imap4.terra.es",
				port: 143,
				encrypt: "none",
				username: "%email%"
			},
			pop3: {
				host: "pop3.terra.es",
				port: 110,
				encrypt: "none",
				username: "%email%"
			}
		},
		"thinline.cz": {
			imap: {
				host: "mail.cesky-hosting.cz",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "mail.cesky-hosting.cz",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			}
		},
		"tiki.ne.jp": {
			pop3: {
				host: "mx.tiki.ne.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"tiscali.cz": {
			pop3: {
				host: "pop3.mail.tiscali.cz",
				port: 110,
				encrypt: "none",
				username: "%email%"
			},
			domains: ["wo.cz","worldonline.cz","worldmail.cz"]
		},
		"tiscali.it": {
			imap: {
				host: "imap.tiscali.it",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "pop.tiscali.it",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			}
		},
		"tmail.plala.or.jp": {
			pop3: {
				host: "tmail.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"toccata.plala.or.jp": {
			pop3: {
				host: "toccata.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"topaz.plala.or.jp": {
			pop3: {
				host: "topaz.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"trio.plala.or.jp": {
			pop3: {
				host: "trio.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"umail.plala.or.jp": {
			pop3: {
				host: "umail.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"umich.edu": {
			imap: {
				host: "mail.umich.edu",
				port: 993,
				encrypt: "ssl",
				username: "%login%"
			}
		},
		"uol.com.br": {
			imap: {
				host: "imap.uol.com.br",
				port: 993,
				encrypt: "ssl",
				username: "%login%"
			},
			pop3: {
				host: "pop3.uol.com.br",
				port: 995,
				encrypt: "ssl",
				username: "%login%"
			}
		},
		"verizon.net": {
			pop3: {
				host: "pop.verizon.net",
				port: 995,
				encrypt: "ssl",
				username: "%login%"
			}
		},
		"violet.plala.or.jp": {
			pop3: {
				host: "violet.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"virgin.net": {
			imap: {
				host: "imap4.virgin.net",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "pop.virgin.net",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			}
		},
		"virginmedia.com": {
			imap: {
				host: "imap.virginmedia.com",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "pop3.virginmedia.com",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			}
		},
		"vm.aikis.or.jp": {
			pop3: {
				host: "mail.aikis.or.jp",
				port: 995,
				encrypt: "ssl",
				username: "%login%"
			}
		},
		"vmail.plala.or.jp": {
			pop3: {
				host: "vmail.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"vp.tiki.ne.jp": {
			pop3: {
				host: "vp.tiki.ne.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"waltz.plala.or.jp": {
			pop3: {
				host: "waltz.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"wave.plala.or.jp": {
			pop3: {
				host: "wave.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"web.de": {
			imap: {
				host: "imap.web.de",
				port: 993,
				encrypt: "ssl",
				username: "%login%"
			},
			pop3: {
				host: "pop3.web.de",
				port: 995,
				encrypt: "ssl",
				username: "%login%"
			}
		},
		"webhuset.no": {
			imap: {
				host: "imap.webhuset.no",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "mail.webhuset.no",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			}
		},
		"websupport.sk": {
			imap: {
				host: "imap.websupport.sk",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "pop.websupport.sk",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			}
		},
		"white.plala.or.jp": {
			pop3: {
				host: "white.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"wine.plala.or.jp": {
			pop3: {
				host: "wine.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"wmail.plala.or.jp": {
			pop3: {
				host: "wmail.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"wp.pl": {
			imap: {
				host: "imap.wp.pl",
				port: 993,
				encrypt: "ssl",
				username: "%login%"
			},
			pop3: {
				host: "pop3.wp.pl",
				port: 995,
				encrypt: "ssl",
				username: "%login%"
			}
		},
		"xmail.plala.or.jp": {
			pop3: {
				host: "xmail.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"xp.wind.jp": {
			pop3: {
				host: "xp.wind.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"xpost.plala.or.jp": {
			pop3: {
				host: "xpost.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"xs4all.nl": {
			pop3: {
				host: "pops.xs4all.nl",
				port: 995,
				encrypt: "ssl",
				username: "%login%"
			}
		},
		"xtra.co.nz": {
			pop3: {
				host: "pop3.xtra.co.nz",
				port: 995,
				encrypt: "ssl",
				username: "%login%"
			}
		},
		"yahoo.co.jp": {
			pop3: {
				host: "pop.mail.yahoo.co.jp",
				port: 995,
				encrypt: "ssl",
				username: "%login%"
			}
		},
		"ybb.ne.jp": {
			pop3: {
				host: "ybbpop.mail.yahoo.co.jp",
				port: 995,
				encrypt: "ssl",
				username: "%login%"
			}
		},
		"yeah.net": {
			imap: {
				host: "imap.yeah.net",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			}
		},
		"yellow.plala.or.jp": {
			pop3: {
				host: "yellow.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"ymail.plala.or.jp": {
			pop3: {
				host: "ymail.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"ypost.plala.or.jp": {
			pop3: {
				host: "ypost.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"zeelandnet.nl": {
			imap: {
				host: "mail.zeelandnet.nl",
				port: 993,
				encrypt: "ssl",
				username: "%login%"
			},
			pop3: {
				host: "mail.zeelandnet.nl",
				port: 995,
				encrypt: "ssl",
				username: "%login%"
			}
		},
		"zmail.plala.or.jp": {
			pop3: {
				host: "zmail.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		},
		"zoho.com": {
			imap: {
				host: "imap.zoho.com",
				port: 993,
				encrypt: "ssl",
				username: "%email%"
			},
			pop3: {
				host: "pop.zoho.com",
				port: 995,
				encrypt: "ssl",
				username: "%email%"
			},
			domains: ["zohomail.com"]
		},
		"zpost.plala.or.jp": {
			pop3: {
				host: "zpost.mail.plala.or.jp",
				port: 110,
				encrypt: "none",
				username: "%login%"
			}
		}
	};
};