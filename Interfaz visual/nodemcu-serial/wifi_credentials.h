/*
 * TFG de Pilar Navarro Garcia
 * 
 * Certificados.
 */

// Wifi de mi casa
const char* ssid = "CW_92147";
const char* password = "vQgGsaCR";

const char* awsEndpoint = "ax6oglwdbc20d-ats.iot.eu-central-1.amazonaws.com";

// certificate pem crt
const String certificatePemCrt = \
//-----BEGIN CERTIFICATE-----
"MIIDWjCCAkKgAwIBAgIVAPGqdVdQx7Sim56Mq5Eqe2hE1022MA0GCSqGSIb3DQEB" \
"CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t" \
"IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yMTExMDgxMjI1" \
"MjdaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh" \
"dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDhzpPYE3XLJcX1C8/q" \
"3ERHYIWsyP2YTBPgK1QqLy94ikoZlfQlGduXbM092gFVvT+MaL6CYN99fwu7xF06" \
"nr4XDKCa7nari1wPN91QCMTaUvByeNhFjUFJdlaR9/yhojHp5hOtURKDA0N+R9y7" \
"wqdjVG2/ndfhze5N35xg7IdWzA0uoHf7aolzmZhKagSfEtnrToNepkxg0a19zyKp" \
"Z6KTERW5vZKqbg4WqhKl08CINtQIOriPfROZEH/D44C5v9jBkPrPFoOCvWnwzcvc" \
"ZLfkGb81LIEXSbkk/azqLXsLKCFN3Mc26q3KQP6DdafLuGcIn5RpgUgj++10Xrx7" \
"thORAgMBAAGjYDBeMB8GA1UdIwQYMBaAFKduFwEE+RANvndcdr7QaOwaLchvMB0G" \
"A1UdDgQWBBRgwjh49Ru7ogD349p78zAOhuBinzAMBgNVHRMBAf8EAjAAMA4GA1Ud" \
"DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAE2eN/mTsTelTEJyFwVaK2aT8" \
"yDxqGhS9U/6gYQpdlj09RRCt9ciOhWY4FbqD5S3Fh6eB1bmlVRpwxlcXD9t1cxQ5" \
"sgI/FMxxceiROZD0/kE/y+bAlMleCZ9WtnFEwm4T8ULSAA9KgZDAQaSwJVD+ySoj" \
"TLYnjcSGwi8SvQKpEIIptSNgzWyuZ4gF9U20vyVDteeOew1egr6A1N9jkjW4s5Sr" \
"7DbhuBpkRXhsUzBqpjVHRg1uI87xMbeIhQfeJaPDJlOkty+SsVjzWsSVzpcDP4Yb" \
"hBMt9HHuZiWfUWUsFJufCn4cD7hYCCwFqsOc1TryXfW0cBDnvYuPlO59PmJeUQ==";
//-----END CERTIFICATE-----

// private pem key
const String privatePemKey = \
//-----BEGIN RSA PRIVATE KEY-----
"MIIEowIBAAKCAQEA4c6T2BN1yyXF9QvP6txER2CFrMj9mEwT4CtUKi8veIpKGZX0" \
"JRnbl2zNPdoBVb0/jGi+gmDffX8Lu8RdOp6+Fwygmu52q4tcDzfdUAjE2lLwcnjY" \
"RY1BSXZWkff8oaIx6eYTrVESgwNDfkfcu8KnY1Rtv53X4c3uTd+cYOyHVswNLqB3" \
"+2qJc5mYSmoEnxLZ606DXqZMYNGtfc8iqWeikxEVub2Sqm4OFqoSpdPAiDbUCDq4" \
"j30TmRB/w+OAub/YwZD6zxaDgr1p8M3L3GS35Bm/NSyBF0m5JP2s6i17CyghTdzH" \
"NuqtykD+g3Wny7hnCJ+UaYFII/vtdF68e7YTkQIDAQABAoIBADrN3EyaDMQBq0K8" \
"d8H26MUZytYLZZdCwK5ROlLeBOPER7XQCkubJ0eQ6wYffteAOEttf6deaPU6n6if" \
"Y5P/gQtpm22dikP0P94qszTDg9hDXyDAfbnnJkhmS/cpeJ3zKkJ6S0eIZGS/9dSu" \
"DDrNX2ALvaT4Cx9fzJ51DDllf31Hmx+mwtbeKzPMW/8SIOkI+iMoIo08pusNaWAv" \
"Dm7j2xJm5L/rK9pHdCDy4ut1FSEvaWhd/8egei3ekmdp+M242cOuj+NupBMAN3px" \
"ljp2Gj/kCqLkmndsErwjGVZXdqSlrpFlFOzGeYOYygHKc0K3mkXTnmVNmWDKa7bn" \
"XhlsxgECgYEA+Ihfc12dkjI8h95VhpAnmiP7WIe5vpvdUUQ+atiiILOFzndtlN2P" \
"Yy2jXC5teyKuVZvkV0+aIsNccFpEuFAdmYcq1KkeZQTG25AwKpN/NhGUsR8CUe+3" \
"Wn01T1TTS1IlJHAWLc65ZCSpg5z8R/S1pA0bJjjF4VQTF8d36QH1BmECgYEA6Jdn" \
"71ZfNkEs1AMM25vnIvonoKgbXZRpGLedydxoLgGIrl6tl899enhc+mFCiHD4RxuK" \
"+01aNNLAjQU04bTsKIK1LIX1m0eswBtFaBrq3ckgJOMaKLv8WvZah9S6on+xYtkh" \
"kVMFnoOc4stoZNjnHgoNvAKQIdpG/sjjz6OHuzECgYB/DEevE+Ye/CtRUWjrFvoH" \
"o+kzAn8DX24twvMdb4zBpsP6HbcY6IIKMyRVVS6GSd5QyRZbQ8gJiVDR5nNGvT3E" \
"ZAKTH7j37w8w4m6c9TWMkEQwnydyd1aFnYgI+9XfpShzjhDobcr71HoLW87jgcbx" \
"C/oa+d6+J1pzR3aW2HdHQQKBgQDD4k98OAezz6wLWg5IHcLJqFgba7iwVT81u949" \
"lUyAAna0M7SLOAn7TMcxbxvzgQD4EfA4uOqRtjTID2x78j052OrwdllkzzZkDBOm" \
"G5VV9/8EUedECx37FaMk5jnQhS5Fi4xDseH3n+xO7PQcirgKinkKsDXm7MgB5/pE" \
"cvrq4QKBgHAXqYtwVHHO+C/insbgce8/+q6hi7gw7t7UHe6cpiIq1+7H/Xo2hdIR" \
"juCEwg3uV/e3+vTEuAOabjBw/+rtDRLCjSDNRym87LNHLX5l/5qA58brcv7a8KRu" \
"zbvZM2KDNnnGVoyvTeMPd4AFz4rOcDErzt7QvhlKfJftkvUz+B/A";
//-----END RSA PRIVATE KEY-----

// AWS IoT CA Certificate //// CA1.pem
const String caPemCrt = \
//-----BEGIN CERTIFICATE-----
"MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF" \
"ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6" \
"b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL" \
"MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv" \
"b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj" \
"ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM" \
"9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw" \
"IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6" \
"VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L" \
"93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm" \
"jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC" \
"AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA" \
"A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI" \
"U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs" \
"N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv" \
"o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU" \
"5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy" \
"rqXRfboQnoZsG4q5WTP468SQvvG5";
//-----END CERTIFICATE-----