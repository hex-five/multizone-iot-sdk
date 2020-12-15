/* Copyright(C) 2020 Hex Five Security, Inc. - All Rights Reserved */

static const u8_t ca_crt[] = {
	0x30, 0x82, 0x02, 0x53, 0x30, 0x82, 0x01, 0xf9, 0xa0, 0x03, 0x02, 0x01,
	0x02, 0x02, 0x14, 0x38, 0x7e, 0x79, 0x48, 0xba, 0x12, 0x30, 0xa4, 0x73,
	0x4c, 0xdf, 0xb9, 0x18, 0x22, 0xd9, 0xd7, 0x64, 0xf8, 0xb2, 0x5e, 0x30,
	0x0a, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x04, 0x03, 0x02, 0x30,
	0x7f, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02,
	0x55, 0x53, 0x31, 0x13, 0x30, 0x11, 0x06, 0x03, 0x55, 0x04, 0x08, 0x0c,
	0x0a, 0x43, 0x61, 0x6c, 0x69, 0x66, 0x6f, 0x72, 0x6e, 0x69, 0x61, 0x31,
	0x15, 0x30, 0x13, 0x06, 0x03, 0x55, 0x04, 0x07, 0x0c, 0x0c, 0x52, 0x65,
	0x64, 0x77, 0x6f, 0x6f, 0x64, 0x20, 0x43, 0x69, 0x74, 0x79, 0x31, 0x20,
	0x30, 0x1e, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x0c, 0x17, 0x48, 0x65, 0x78,
	0x20, 0x46, 0x69, 0x76, 0x65, 0x20, 0x53, 0x65, 0x63, 0x75, 0x72, 0x69,
	0x74, 0x79, 0x2c, 0x20, 0x49, 0x6e, 0x63, 0x2e, 0x31, 0x22, 0x30, 0x20,
	0x06, 0x03, 0x55, 0x04, 0x03, 0x0c, 0x19, 0x48, 0x65, 0x78, 0x20, 0x46,
	0x69, 0x76, 0x65, 0x20, 0x53, 0x65, 0x63, 0x75, 0x72, 0x69, 0x74, 0x79,
	0x20, 0x52, 0x6f, 0x6f, 0x74, 0x20, 0x43, 0x41, 0x30, 0x1e, 0x17, 0x0d,
	0x32, 0x30, 0x30, 0x36, 0x32, 0x31, 0x32, 0x32, 0x33, 0x30, 0x33, 0x36,
	0x5a, 0x17, 0x0d, 0x33, 0x30, 0x30, 0x36, 0x31, 0x39, 0x32, 0x32, 0x33,
	0x30, 0x33, 0x36, 0x5a, 0x30, 0x7f, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03,
	0x55, 0x04, 0x06, 0x13, 0x02, 0x55, 0x53, 0x31, 0x13, 0x30, 0x11, 0x06,
	0x03, 0x55, 0x04, 0x08, 0x0c, 0x0a, 0x43, 0x61, 0x6c, 0x69, 0x66, 0x6f,
	0x72, 0x6e, 0x69, 0x61, 0x31, 0x15, 0x30, 0x13, 0x06, 0x03, 0x55, 0x04,
	0x07, 0x0c, 0x0c, 0x52, 0x65, 0x64, 0x77, 0x6f, 0x6f, 0x64, 0x20, 0x43,
	0x69, 0x74, 0x79, 0x31, 0x20, 0x30, 0x1e, 0x06, 0x03, 0x55, 0x04, 0x0a,
	0x0c, 0x17, 0x48, 0x65, 0x78, 0x20, 0x46, 0x69, 0x76, 0x65, 0x20, 0x53,
	0x65, 0x63, 0x75, 0x72, 0x69, 0x74, 0x79, 0x2c, 0x20, 0x49, 0x6e, 0x63,
	0x2e, 0x31, 0x22, 0x30, 0x20, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0c, 0x19,
	0x48, 0x65, 0x78, 0x20, 0x46, 0x69, 0x76, 0x65, 0x20, 0x53, 0x65, 0x63,
	0x75, 0x72, 0x69, 0x74, 0x79, 0x20, 0x52, 0x6f, 0x6f, 0x74, 0x20, 0x43,
	0x41, 0x30, 0x59, 0x30, 0x13, 0x06, 0x07, 0x2a, 0x86, 0x48, 0xce, 0x3d,
	0x02, 0x01, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x03, 0x01, 0x07,
	0x03, 0x42, 0x00, 0x04, 0x09, 0x2c, 0x6e, 0xbb, 0xc5, 0x6a, 0xdb, 0x37,
	0x6f, 0xad, 0x96, 0x4b, 0x6b, 0xce, 0x2c, 0xbf, 0x8d, 0x93, 0xa4, 0x43,
	0x6c, 0xc9, 0x9c, 0x66, 0x57, 0x53, 0x2f, 0x93, 0x04, 0xeb, 0xce, 0x96,
	0xa2, 0x21, 0x93, 0x67, 0xf6, 0x32, 0x47, 0xf3, 0x31, 0x76, 0x24, 0x3c,
	0x1a, 0x5a, 0xb7, 0x48, 0x84, 0xe3, 0xd9, 0xd4, 0x81, 0x63, 0x74, 0x39,
	0xfe, 0x23, 0x69, 0xa4, 0x5f, 0x29, 0x66, 0x76, 0xa3, 0x53, 0x30, 0x51,
	0x30, 0x1d, 0x06, 0x03, 0x55, 0x1d, 0x0e, 0x04, 0x16, 0x04, 0x14, 0xd2,
	0x5f, 0xc2, 0x0b, 0xf5, 0xe2, 0xbb, 0x48, 0x7f, 0xa6, 0xed, 0xa8, 0xb6,
	0xa9, 0x17, 0x7c, 0xc7, 0x88, 0x37, 0x41, 0x30, 0x1f, 0x06, 0x03, 0x55,
	0x1d, 0x23, 0x04, 0x18, 0x30, 0x16, 0x80, 0x14, 0xd2, 0x5f, 0xc2, 0x0b,
	0xf5, 0xe2, 0xbb, 0x48, 0x7f, 0xa6, 0xed, 0xa8, 0xb6, 0xa9, 0x17, 0x7c,
	0xc7, 0x88, 0x37, 0x41, 0x30, 0x0f, 0x06, 0x03, 0x55, 0x1d, 0x13, 0x01,
	0x01, 0xff, 0x04, 0x05, 0x30, 0x03, 0x01, 0x01, 0xff, 0x30, 0x0a, 0x06,
	0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x04, 0x03, 0x02, 0x03, 0x48, 0x00,
	0x30, 0x45, 0x02, 0x20, 0x5e, 0x2e, 0x99, 0x68, 0xee, 0x97, 0x40, 0x7c,
	0xc1, 0xb0, 0x52, 0x56, 0x90, 0xc2, 0xbc, 0x31, 0x4d, 0x25, 0x7c, 0x51,
	0x16, 0x12, 0x8c, 0x08, 0xa7, 0xd9, 0x39, 0xe4, 0x70, 0x68, 0x86, 0x94,
	0x02, 0x21, 0x00, 0xd2, 0x3d, 0xa9, 0xa4, 0x41, 0xd9, 0x75, 0xc3, 0xf1,
	0xd7, 0x97, 0x8e, 0xfc, 0x9e, 0x26, 0x7f, 0xdd, 0xd4, 0xa4, 0xfd, 0x9b,
	0x5b, 0x3f, 0xc6, 0xc5, 0x7e, 0x36, 0x4d, 0x57, 0x9b, 0x8f, 0xa1
};

/*	// prime256v1
	"-----BEGIN CERTIFICATE-----\n"
	"MIICUzCCAfmgAwIBAgIUOH55SLoSMKRzTN+5GCLZ12T4sl4wCgYIKoZIzj0EAwIw\n"
	"fzELMAkGA1UEBhMCVVMxEzARBgNVBAgMCkNhbGlmb3JuaWExFTATBgNVBAcMDFJl\n"
	"ZHdvb2QgQ2l0eTEgMB4GA1UECgwXSGV4IEZpdmUgU2VjdXJpdHksIEluYy4xIjAg\n"
	"BgNVBAMMGUhleCBGaXZlIFNlY3VyaXR5IFJvb3QgQ0EwHhcNMjAwNjIxMjIzMDM2\n"
	"WhcNMzAwNjE5MjIzMDM2WjB/MQswCQYDVQQGEwJVUzETMBEGA1UECAwKQ2FsaWZv\n"
	"cm5pYTEVMBMGA1UEBwwMUmVkd29vZCBDaXR5MSAwHgYDVQQKDBdIZXggRml2ZSBT\n"
	"ZWN1cml0eSwgSW5jLjEiMCAGA1UEAwwZSGV4IEZpdmUgU2VjdXJpdHkgUm9vdCBD\n"
	"QTBZMBMGByqGSM49AgEGCCqGSM49AwEHA0IABAksbrvFats3b62WS2vOLL+Nk6RD\n"
	"bMmcZldTL5ME686WoiGTZ/YyR/MxdiQ8Glq3SITj2dSBY3Q5/iNppF8pZnajUzBR\n"
	"MB0GA1UdDgQWBBTSX8IL9eK7SH+m7ai2qRd8x4g3QTAfBgNVHSMEGDAWgBTSX8IL\n"
	"9eK7SH+m7ai2qRd8x4g3QTAPBgNVHRMBAf8EBTADAQH/MAoGCCqGSM49BAMCA0gA\n"
	"MEUCIF4umWjul0B8wbBSVpDCvDFNJXxRFhKMCKfZOeRwaIaUAiEA0j2ppEHZdcPx\n"
	"15eO/J4mf93UpP2bWz/GxX42TVebj6E=\n"
	"-----END CERTIFICATE-----\n"
	;*/

/*	// rsa-2048
	"-----BEGIN CERTIFICATE-----\n"
	"MIID3zCCAsegAwIBAgIUYqqB7o4HICFS1jydTqwguW8eYEEwDQYJKoZIhvcNAQEL\n"
	"BQAwfzELMAkGA1UEBhMCVVMxEzARBgNVBAgMCkNhbGlmb3JuaWExFTATBgNVBAcM\n"
	"DFJlZHdvb2QgQ2l0eTEgMB4GA1UECgwXSGV4IEZpdmUgU2VjdXJpdHksIEluYy4x\n"
	"IjAgBgNVBAMMGUhleCBGaXZlIFNlY3VyaXR5IFJvb3QgQ0EwHhcNMjAwNjIyMDIz\n"
	"NTEyWhcNMzAwNjIwMDIzNTEyWjB/MQswCQYDVQQGEwJVUzETMBEGA1UECAwKQ2Fs\n"
	"aWZvcm5pYTEVMBMGA1UEBwwMUmVkd29vZCBDaXR5MSAwHgYDVQQKDBdIZXggRml2\n"
	"ZSBTZWN1cml0eSwgSW5jLjEiMCAGA1UEAwwZSGV4IEZpdmUgU2VjdXJpdHkgUm9v\n"
	"dCBDQTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAMen7QzLtWJNomwo\n"
	"RQ0eYvdUQAolMHEskUanl67KNIYI61vO2b0/Z+XAlBJTzwwJfy3ZOVWIKq7ajhQR\n"
	"GTk0aRGxlQMxCl6/a0nDeYAR9NPi2Ztqq8SAsqN/Fq0CGQsBym+gEJCvKkDn0909\n"
	"RVW04U1JVl++aCVHC07ERgFkfXmsbpIJlCCwJABid4lCqZCP5CVwFn7B58wZYzKT\n"
	"1EOjOjmFJF4r/3c3Lom3yesdM9rGKauwr/1KwwmqJtSDRERzCYoAKvYezHIKAjcy\n"
	"L/63InGVXPSi1aOiptLO8ZIk2BPrSqPEQ7l68gsg0jMvf4IVa1Wxg8FMOBQWbBJL\n"
	"3dsrpJkCAwEAAaNTMFEwHQYDVR0OBBYEFPkzMOaNdi0O6QSaW7s5Bsl+hbzaMB8G\n"
	"A1UdIwQYMBaAFPkzMOaNdi0O6QSaW7s5Bsl+hbzaMA8GA1UdEwEB/wQFMAMBAf8w\n"
	"DQYJKoZIhvcNAQELBQADggEBABr4CQOq8wPEgWvdiHwkIM5tjMEZhIsGPkgmj/xy\n"
	"2/xHxhJonZvzlwOgw8sdA4eaK2bhwDxYkJlyIvxrx6JgJz9B13m0qIsV0xIHa+Qz\n"
	"/B4/Kr5StJgCqB+99JgIbNpYf5jnICT2axNANh/5IffuKzjDhr+8Dwcqun61gGwR\n"
	"W7I3QXdkHRv1wMLlohHIH0j76et18gna9O7lr+z+VSBRejsgxWojD01GvNNfPzrW\n"
	"HLPPhd7SGeTPPPCJrdADU10WRwR+bGBCMLD/zpCJ0o5K6YW6V1oqC4xttZtyvY6j\n"
	"CfzNPO1zecoNnVlPtEz82Hq9LSqbP2E1gy+ZuASol/iwM+A=\n"
	"-----END CERTIFICATE-----\n"
;
*/
