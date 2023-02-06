add:
	curl -X POST http://localhost:7000/api/v1/person/add -H "Content-Length: 250" -d \
		"{ \
		\"firstName\": \"Christopher\", \
		\"lastName\": \"Chicken\", \
		\"age\": 45 \
		}"

get:
	curl -X GET http://localhost:7000/api/v1/person/getAll
