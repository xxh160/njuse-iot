KUIPER=$1
VALUEDESCRIPTOR=$2
IDX=$3
URL=$4

STREAM=demo

# create rule
curl -X POST \
	 -H 'Content-Type: application/json' \
	 -d "{
			\"id\": \"led_rule$IDX\",
			\"sql\": \"select $VALUEDESCRIPTOR from $STREAM where $VALUEDESCRIPTOR = $IDX\",
			\"actions\": [
				{
					\"rest\": {
						\"method\": \"put\",
						\"dataTemplate\": \"{\\\"$VALUEDESCRIPTOR\\\": $IDX}\",
						\"sendSingle\": true,
						\"url\": \"$URL\"
					}
				},
				{
					\"log\": {}
				}
			]
		}" \
	 "$KUIPER/rules"
