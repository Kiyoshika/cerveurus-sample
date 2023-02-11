# Cerveurus Sample
This is a sample project using my [Cerveurus](https://github.com/Kiyoshika/cerveurus) library.

This is a simple API that maintains a list of people (first name, last name and age) which you can add, delete and fetch the list.

I've created a Makefile that shows an example of adding a person and fetching the list (`make add` and `make get`).

# Endpoints

## Add Person
* URI: `/api/v1/person/add`
* REQUEST BODY:
```json
{
	"firstName": "...",
	"lastName": "...",
	"age": 40
}
```
* If person already exists (first and last name match), returns status `400 BAD REQUEST`.
* If JSON fails to parse, returns status `400 BAD REQUEST`
* Otherwise, returns status `200 OK`

## Get List of People
* URI: `/api/v1/person/getAll`
* RESPONSE BODY:
```json
{
	"people": [
		{
			"firstName": "...",
			"lastName": "...",
			"age": 40
		},

		{
			...
		}

		...

	]
}
```
* Returns status `200 OK`

## Delete Person
* URI: `/api/v1/person/delete`
* REQUEST BODY:
```json
{
	"firstName": "...",
	"lastName": "...",
	"age": 40
}
```
* If person doesn't exist, returns `404 NOT FOUND`
* Otherwise, returns `200 OK`
