/* Copyright(C) 2020 Hex Five Security, Inc. - All Rights Reserved */

static const u8_t cli_key[] = { /* mqtt-client-001.key */
	0x30, 0x77, 0x02, 0x01, 0x01, 0x04, 0x20, 0x40, 0xa5, 0xc2, 0xce, 0x6e,
	0x23, 0x3d, 0x6f, 0x00, 0xce, 0x6e, 0xc3, 0x7c, 0xa5, 0x61, 0xa0, 0x01,
	0x8c, 0x34, 0x12, 0xce, 0x17, 0x4c, 0xb2, 0x7f, 0xf1, 0x3f, 0x3b, 0xb7,
	0x14, 0x44, 0xe5, 0xa0, 0x0a, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d,
	0x03, 0x01, 0x07, 0xa1, 0x44, 0x03, 0x42, 0x00, 0x04, 0x70, 0x33, 0xcb,
	0x00, 0x06, 0x37, 0xd8, 0x3b, 0x93, 0x75, 0x8a, 0x26, 0xf5, 0xff, 0x1f,
	0x94, 0x07, 0xdb, 0x7b, 0x43, 0x0d, 0xe0, 0x6f, 0xa6, 0x4a, 0xdd, 0x70,
	0xfa, 0xe4, 0x82, 0x71, 0x2c, 0xa2, 0xa7, 0x90, 0x7c, 0x20, 0x9b, 0x3c,
	0xe5, 0x67, 0xd3, 0xc8, 0x5b, 0x59, 0x82, 0x10, 0xac, 0x5f, 0x9a, 0xa0,
	0x17, 0x0d, 0x71, 0x25, 0xfc, 0x86, 0x99, 0x26, 0x08, 0x7a, 0xcb, 0x3c,
	0xaf
};

/*	// prime256v1
	"-----BEGIN PRIVATE KEY-----\n"
	"MIGHAgEAMBMGByqGSM49AgEGCCqGSM49AwEHBG0wawIBAQQgQKXCzm4jPW8Azm7D\n"
	"fKVhoAGMNBLOF0yyf/E/O7cUROWhRANCAARwM8sABjfYO5N1iib1/x+UB9t7Qw3g\n"
	"b6ZK3XD65IJxLKKnkHwgmzzlZ9PIW1mCEKxfmqAXDXEl/IaZJgh6yzyv\n"
	"-----END PRIVATE KEY-----\n"
;*/
	/*// rsa-2048
	"-----BEGIN PRIVATE KEY-----\n"
	"MIIEvgIBADANBgkqhkiG9w0BAQEFAASCBKgwggSkAgEAAoIBAQDSuEV+vIyMqCXz\n"
	"EZAA+gOAERuOvs3o1RoY6oU39ETp55kJ3TPUZUzlAiHQ+vYcEAow9QltvI61MeA8\n"
	"HVQihRR4AvelNitbFJR2n5O46LlVrjKGadre9VZdcmMKZcGtOCO0m8FKsJxqni3B\n"
	"VtVl/giTu+dZgvpY5MhgpTwLttNRE6txXiBpmlL9EuLt0kxsuZKjUNMYMfGRgHcj\n"
	"Vue4pCypNgKsw7spiQ38gLRQFcjpgQSj9NqetTzWNRzSBYiey7U5aDwRJWhqRCgG\n"
	"m9lVVUVKjaSjIY7BmtaxNlq164rFlAFSb4cIE4ejVVugloNbCn92TOp8MENsGFjd\n"
	"+2g3z1J/AgMBAAECggEAVdQkylh2Txx2SKlQpHUxU8qkFOJ28G1McfWkSYQ/RZ7F\n"
	"4RSudcbFKUpTB9IQyKFn5UUJkB8LaZZlJVc029Kk4bUjtbF4u2KIjkC1ZM22H8YS\n"
	"Yllfgw0fHnTnDmcgGjQRpxECDDAfpfXD4UpDJYXxbGtO8lC9M9QMIgFrkcHoFM9W\n"
	"xBv8QiLlMTrWFhTNL9hl0L3gXYvGv0ilTgthIAUF5WKeu+byne8qa52nkWxFiG4O\n"
	"gChTebeqe2NwHR3W2QfkluRKuSA11YO/CYhwFJxUY4Usp6NeT4edE6zc5+yDds3C\n"
	"p6MSVDydh88pjQL9xfPQIFpqpdCY/n64De4vacPesQKBgQD0Xq0ltIhA/A3xB34P\n"
	"ZaPCOsyaHnnCtso0owE7hhspUZ3F3nrZpbVh9NS5BaqQKnGFAJQ4pA/C41l0Efea\n"
	"N6b00o2guyaZ7f0qrHOLsav7Pqt1UtIsADC5x4+wZfrjhN7pJ5t024h9KNIOczeh\n"
	"hgJIhZdLVIZBaf70o1YEHQaFBQKBgQDcv50zbCLIaGas4UuYVs19AFAKmlp79+Qd\n"
	"RAGEbFL7m8uGTMNN1kkDj8Fu3HC3nhft08L0ZYcJBh2p23uV6mgd10LurhnSjeh2\n"
	"saTdNpkSgGiFsbUoOkor/snROLzGwOC8IGqFm6Bo+d0cJo5xHpLPnTs91Oh9WLOA\n"
	"Ep1ItzAQswKBgQCQb6VHs/0U6+YRoKwRzbGNC9QDBCBTxlad5frEg+9p7KOkYGuc\n"
	"WSVsUJmrAn5d17hhNIZnNBFcz6P/xt7ieCJmckzjw7p9IP2SS3vABbPxPIl7Pwia\n"
	"mUIzjJCRYrV35QEcLhzkZR23XHlLsS4gemS/GLbj/4GbCFt5nVEp/T3tQQKBgBga\n"
	"hBu9Tv77B1sRLqPvtnhTtGmz+PVYV/wqFNBU06rNlCeR8jFpmIdfBwwH3fbuuEYr\n"
	"vTnabCg8FrkZUiFIex3nB1PwhrlK7XsoYowVDhnLE56ljNZbpwWuyHKj81jwbpmc\n"
	"IIV+PMG5pr8QJrWLVRR+Yp2ZxdkJ+m+dmLb76a+pAoGBALQ0V1RyxneTQi6zjQE/\n"
	"zjiaY+T3bhvnMrNWM2zQmDXGxFtPIV7Ib07wB7+nb706sQbsLreRqCnus/7N8Eg+\n"
	"+WlgrsdRXugjZgfH3yFT+FvY+0NgCUgR0lf/pRMMW+LrcBGALc9fhbdDO7aKXx1A\n"
	"8pvmrasXyulLQLDSr6AQeAsX\n"
	"-----END PRIVATE KEY-----\n"
	;*/
