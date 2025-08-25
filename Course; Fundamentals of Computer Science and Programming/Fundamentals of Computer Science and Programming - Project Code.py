import json
class Aasan_Pardakht:
    def __init__(person):
        person.Accounts = {}
    def data(person):
        with open("data.json", "w") as file:
            json.dump(person.Accounts, file)
    def data_1(person):
        try:
            with open("data.json", "r") as file:
                person.Accounts = json.load(file)
        except FileNotFoundError:
            person.data()
    def Account_Registration(person):
        Name = input("Enter your Name: ")
        Card_Number = input("Enter your Card_Number: ")
        PIN = input("Enter your PIN: ")
        Phone_Number = input("Enter your Phone_Number: ")

        if Phone_Number not in person.Accounts:
            person.Accounts[Phone_Number] = {"Name": Name, "Card_Number": Card_Number, "PIN": PIN, "Card_Balance": 100000, "Charge_Balance": 0}
            person.data()
            print("You have Registered Successfully")
        else:
            print("Your Phone number is already Registered")
    def Charging(person):
        Phone_Number = input("Enter your Phone_Number: ")
        if Phone_Number in person.Accounts:
            Amount = int(input("Enter the Amount: "))

            if Amount > 0:
                person.Accounts[Phone_Number]["Charge_Balance"] += Amount
                person.data()
                print("Charge is made")
            else:
                print("The Amount is not Acceptable")
        else:
            print("Wrong Phone_Number")
    def Card_to_Card(person):
        Sender_Phone_Number = input("Enter the Sender_Phone_Number: ")
        Receiver_Phone_Number = input("Enter the Receiver_Phone_Number: ")
        Amount = int(input("Enter the Amount to be Transferred: "))

        if Sender_Phone_Number in person.Accounts and Receiver_Phone_Number in person.Accounts:
            if person.Accounts[Sender_Phone_Number]["Card_Balance"] >= Amount:
                person.Accounts[Sender_Phone_Number]["Card_Balance"] -= Amount
                person.Accounts[Receiver_Phone_Number]["Card_Balance"] += Amount
                person.data()
                print("Transfer is made")
            else:
                print("The Operation cant be Done")
        else:
            print("Wrong Phone_Number")
    def Balance_Checking(person):
        Phone_Number = input("Enter your Phone_Number: ")

        if Phone_Number in person.Accounts:
            print("Card_Balance: ", person.Accounts[Phone_Number]["Card_Balance"])
            print("Charge_Balance: ", person.Accounts[Phone_Number]["Charge_Balance"])
        else:
            print("Wrong Phone_Number")

    def Login(person):
        Phone_Number = input("Enter your Phone_Number: ")
        PIN = input("Enter your PIN: ")

        if Phone_Number in person.Accounts and PIN == person.Accounts[Phone_Number]["PIN"]:
            print("You have Successfully Logged in")
            return True
        else:
            print("Wrong Phone_Number or PIN")
            return False
    def Main_Page(person):
        while True:
            print("1. Account_Registration")
            print("2. Charging")
            print("3. Card_to_Card")
            print("4. Balance_Checking")
            print("5. Login")
            print("6. Done")

            Which_Number_you_choose = input("Your Number is: ")

            if Which_Number_you_choose == "1":
                person.Account_Registration()

            elif Which_Number_you_choose == "2":
                person.Charging()

            elif Which_Number_you_choose == "3":
                person.Card_to_Card()

            elif Which_Number_you_choose == "4":
                person.Balance_Checking()

            elif Which_Number_you_choose == "5":
                person.Login()

            elif Which_Number_you_choose == "6":
                break
            else:
                print("Wrong Number")

if __name__ == "__main__":
    Aap = Aasan_Pardakht()
    Aap.data_1()
    Aap.Main_Page()