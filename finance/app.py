import os
import datetime
from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd


# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    inventory = db.execute("SELECT * FROM inventory WHERE user_id = ?", session["user_id"])

    totals = []
    prices = []
    for item in inventory:
        price = float(lookup(item["symbol"])["price"])
        prices.append(price)
        share = item["share"]
        total = price * share
        totals.append(total)

    user_money = float(db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"])
    total_money = 0
    for i in totals:
        total_money = total_money + i
    total_money = total_money + user_money

    count = 0
    return render_template("index.html", inventory=inventory, user_money=user_money, total_money = total_money, totals=totals, prices=prices)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        try:
            shares = int(request.form.get("shares"))
        except(ValueError):
            return apology("Please enter a valid value")

        # Symbol Checks
        if symbol == "":
            return apology("Please enter a symbol")
        if lookup(symbol) is None:
            return apology("Please enter a valid symbol")
        # Shares Checks
        if 1 > shares:
            return apology("Please enter a positive value")
        # Getting stock informations and user money
        name = lookup(symbol)["name"]
        sm = lookup(symbol)["symbol"]
        price = float(lookup(symbol)["price"])
        user_money = float(db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"])

        # Checking user have enough money
        if (user_money) < (price * float(shares)):
            return apology("Not enough money")
        # Creating "transactions" and "inventory" Table
        try:
            db.execute("CREATE TABLE transactions (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, user_id INTEGER FOREING KEY REFERENCES users(id), symbol TEXT NOT NULL, name TEXT NOT NULL, share INTEGER, price NUMERIC, datetime DATETIME NOT NULL)")
        except(RuntimeError):
            try:
                db.execute("CREATE TABLE inventory (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, user_id INTEGER FOREING KEY REFERENCES users(id), symbol TEXT NOT NULL, name TEXT NOT NULL, share INTEGER)")
            except(RuntimeError):
                # Adding transaction to db
                db.execute("INSERT INTO transactions (user_id, symbol, name, share, price, datetime) VALUES(?,?,?,?,?,?)",
                           session["user_id"], sm, name, shares, price, datetime.datetime.utcnow().strftime('%Y-%m-%d %H:%M:%S'))

                # Adding/Updating inventory table
                if db.execute("SELECT symbol FROM inventory WHERE symbol = ? AND user_id = ?", sm, session["user_id"]) == []:
                    db.execute("INSERT INTO inventory (user_id, symbol, name, share) VALUES(?,?,?,?)",
                               session["user_id"], sm, name, shares)
                    flash("Stock bought")
                else:
                    current_shares = int(db.execute("SELECT share FROM inventory WHERE symbol = ? AND user_id = ?",
                                         sm, session["user_id"])[0]["share"]) + shares
                    db.execute("UPDATE inventory SET share = ? WHERE symbol = ? AND user_id = ? ",
                               current_shares, sm, session["user_id"])
                    flash("Stock bought")
                # Updating remaning cash
                remaining_money = user_money - (price * float(shares))
                db.execute("UPDATE users SET cash = ? WHERE id = ?", remaining_money, session["user_id"])
        return redirect("/")
    return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    transactions = db.execute("SELECT * FROM transactions WHERE user_id = ?", session["user_id"])

    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        if lookup(symbol) is None:
            return apology("Invalid Symbol")
        name = lookup(symbol)["name"]
        sm = lookup(symbol)["symbol"]
        price = usd(lookup(symbol)["price"])

        return render_template("quoted.html", name=name, sm=sm, price=price)

    return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")
        try:
            users = db.execute("SELECT username FROM users WHERE username = ?", username)
            if username == users[0]["username"]:
                return apology("Username already exist")
        except (ValueError, IndexError):
            if username == "":
                return apology("Please type an Username")
            elif password == "":
                return apology("Please enter a password")
            elif confirmation == "":
                return apology("Please enter your password again!")
            elif password != confirmation:
                return apology("Password doesn't match")
            else:
                db.execute("INSERT INTO users(username, hash) VALUES (?,?)", username, generate_password_hash(password))
                flash("Registered")

            return render_template("login.html")
    else:
        return render_template("register.html")


@app.route("/changepw", methods=["GET", "POST"])
@login_required
def change_password():
    if request.method == "POST":
        password = request.form.get("password")
        newpassword = request.form.get("newpassword")
        confirmation = request.form.get("confirmation")
        rows = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])

        if password == "":
            return apology("Please enter your current password")
        elif not check_password_hash(rows[0]["hash"], password):
            return apology("invalid password", 403)
        elif newpassword == "":
            return apology("Please enter your new password")
        elif confirmation == "":
            return apology("Please enter your new password again!")
        elif newpassword != confirmation:
            return apology("Password doesn't match")
        else:
            db.execute("UPDATE users SET hash = ? WHERE id = ?", generate_password_hash(newpassword), session["user_id"])
            flash("Your password changed")
            return render_template("login.html")
    return render_template("changepw.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    symbols = db.execute("SELECT symbol FROM inventory WHERE user_id = ?", session["user_id"])
    if request.method == "POST":

        symbol = request.form.get("symbol")
        shares = int(request.form.get("shares"))
        name = lookup(symbol)["name"]
        sm = lookup(symbol)["symbol"]
        price = float(lookup(symbol)["price"])
        user_shares = db.execute("SELECT share FROM inventory WHERE user_id = ? AND symbol = ?", session["user_id"], symbol)
        # Shares Checks
        if 1 > shares:
            return apology("Please enter a positive value")
        if shares > user_shares[0]["share"]:
            return apology("Please don't exceed maximum share you own")
        db.execute("INSERT INTO transactions (user_id, symbol, name, share, price, datetime) VALUES(?,?,?,?,?,?)",
                   session["user_id"], sm, name, (-shares), price, datetime.datetime.utcnow().strftime('%Y-%m-%d %H:%M:%S'))
        current_shares = int(db.execute("SELECT share FROM inventory WHERE symbol = ? AND user_id = ?",
                             sm, session["user_id"])[0]["share"]) - shares
        db.execute("UPDATE inventory SET share = ? WHERE symbol = ? AND user_id = ? ", current_shares, sm, session["user_id"])
        flash("Stock sold")
        if current_shares == 0:
            db.execute("DELETE FROM inventory WHERE symbol = ? AND user_id = ?", sm, session["user_id"])
        user_money = float(db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"])
        remaining_money = user_money + (price * float(shares))
        db.execute("UPDATE users SET cash = ? WHERE id = ?", remaining_money, session["user_id"])
        return redirect("/")
    return render_template("sell.html", symbols=symbols)
