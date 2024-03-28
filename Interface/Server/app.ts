import express from "express";
import path from "path";
import fs from "fs";

// App
const app = express();

// Constants
const DB_PATH = path.resolve("db.json");
const PORT = process.env["PORT"] || 3000;

// Middlewares
app.use(express.json());

// routes
app.get("/", async (_req, res) => {
    fs.readFile(DB_PATH, "utf-8", (err, jsonString) => {
        if (err) return console.log("Error in reading from db");
        const values = JSON.parse(jsonString);
        res.status(200).json({
            totalValues: values.length, values,
        });
    });
});

app.post("/", async (req, res) => {
    fs.readFile(DB_PATH, "utf-8", (err, jsonString) => {
        if (err) return console.log("Error in reading from db");
        const body = req.body;
        const valuesArr = JSON.parse(jsonString);
        const obj = {
            light: body.value, timestamp: new Date(),
        };
        valuesArr.push(obj);
        fs.writeFile(DB_PATH, JSON.stringify(valuesArr), (err) => {
            if (err) return console.log("Error in updating db");
            res.status(200).json({
                message: "Values saved", value: valuesArr[valuesArr.length - 1],
            });
        });
    });
});

app.listen(PORT, () => console.log("Listening on port", PORT));