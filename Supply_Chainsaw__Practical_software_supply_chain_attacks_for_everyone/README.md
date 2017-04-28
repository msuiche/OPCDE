#Supply Chainsaw: Practical software supply chain attacks for everyone

I recently presented Supply Chainsaw: Practical software supply chain attacks for everyone at the OPCDE technical security conference in Dubai.

In between pictures of Sharknadoes and memes were an array of software supply chain attacks:

- We proved most programming language package managers have major security weaknesses
	* Typo or wrong command attacks compromise those who make mistakes installing packages
	* Anonymous automatic registration and publishing make attacks easy
	* Weak authentication, no 2-factor, sometimes none at all
	* Developers of popular open-source packages and their powerful credentials are exposed to many different attacks through the development process and permanent credential caching
- Operating system package managers are manual-review and harder to poison
- But nearly every OS is acquired insecurely and unlikely to be verified by the user, from Linux to Mac OS, to Windows
- MITM attacks proven practical against proxy/VPN/Tor users - over 5,000 potential opportunities to poison executable downloads in my test
- We became OS and package mirrors to prove
	* Anyone could infect packages and OS’s delivered via mirror
	* Can be quick, cheap, anonymous, with worldwide effect
	* Packages often are not verified against anything external
	* We were never denied anything we asked for
	* We are running honest mirrors, but we have no guarantee others are trustworthy or have not been compromised
- Between hundreds and millions of users can be compromised using these techniques
- Supply chain attacks are happening in the wild now