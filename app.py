import os
import json
from flask import Flask, render_template, request, flash, redirect, url_for

app = Flask(__name__)
app.secret_key = os.getenv("FLASK_SECRET_KEY", "fallback_key")

jsonfile = "data.json"

def load_data():
    if os.path.exists(jsonfile):
        with open(jsonfile, "r") as reader:
            return json.load(reader)
    else:
        return {
            "absentees": 0,
            "projects": 0,
            "best_performer": "Not Assigned"
        }

def save_data(data):
    with open(jsonfile, "w") as writer:
        json.dump(data, writer)

@app.route("/", methods=["GET", "POST"])
def index():
    data = load_data()

    if request.method == "POST":
        absentees_str = request.form.get("absentees", "").strip()
        projects_str = request.form.get("projects", "").strip()
        performer_name = request.form.get("performer", "").strip()

        try:
            if absentees_str != "":
                absentees_val = int(absentees_str)
                if absentees_val < 0 or absentees_val > 100:
                    flash("Absentees must be between 0 and 100.")
                    return redirect(url_for("index"))
                data["absentees"] = absentees_val

            if projects_str != "":
                projects_val = int(projects_str)
                if projects_val < 0 or projects_val > 50:
                    flash("Projects remaining must be between 0 and 50.")
                    return redirect(url_for("index"))
                data["projects"] = projects_val

            if performer_name != "":
                if len(performer_name) > 50:
                    flash("Best performer name must be 50 characters or less.")
                    return redirect(url_for("index"))
                if not all(c.isalpha() or c.isspace() for c in performer_name):
                    flash("Best performer name can contain only letters and spaces.")
                    return redirect(url_for("index"))
                data["best_performer"] = performer_name

            save_data(data)
            flash("Data updated successfully!")

        except ValueError:
            flash("Invalid input. Please enter valid numbers for absentees and projects.")

    return render_template(
        "index.html",
        absentees=data["absentees"],
        projects=data["projects"],
        performer=data["best_performer"]
    )

@app.route("/reset")
def reset():
    data = {
        "absentees": 0,
        "projects": 0,
        "best_performer": "Not Assigned"
    }
    save_data(data)
    flash("All data has been reset.")
    return redirect(url_for("index"))

@app.route("/api/data", methods=["GET"])
def get_data():
    try:
        data = load_data()
        absentees = data.get("absentees", 0)

        if absentees > 20:
            color = {"r": 255, "g": 0, "b": 0}      # Red
        elif absentees > 10:
            color = {"r": 255, "g": 40, "b": 0}      # orange

        elif absentees > 4:
            color = {"r": 255, "g": 200, "b": 0}    # yellow
        else:
            color = {"r": 0, "g": 255, "b": 0}      # Green

        return {
            "absentees": data["absentees"],
            "projects": data["projects"],
            "best_performer": data["best_performer"],
            "led": color
        }, 200
    except Exception as e:
        return {"error": str(e)}, 500

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000, debug=True)
