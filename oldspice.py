import sys, urllib2, os, sys, time

chiServer = 'http://www-bcf.usc.edu/~chiso/itp125/project_version_1/'
# stores a string value of Chi's mp3 file server

mp3list = [ '0.mp3', '1.mp3', '2.mp3', '3.mp3', '4.mp3',
'5.mp3', '6.mp3', '7.mp3', '8.mp3', '9.mp3', 'f-b1-hello_caller.mp3',
'f-b2-lady_at.mp3', 'f-e1-she_will_get_back_to_you.mp3',
'f-e2-thanks_for_calling.mp3', 'f-r0.1-unable_to_take_call.mp3',
'f-r0.2-she_is_busy.mp3', 'f-r1-ingesting_old_spice.mp3',
'f-r2-listening_to_reading.mp3','f-r3-lobster_dinner.mp3',
'f-r4-moon_kiss.mp3', 'f-r5-riding_a_horse.mp3', 'hello.mp3',
'm-b1-hello.mp3', 'm-b2-have_dialed.mp3', 'm-e1-horse.mp3', 'm-e2-jingle.mp3',
'm-e3-on_phone.mp3', 'm-e4-swan_dive.mp3','m-e5-voicemail.mp3',
'm-leave_a_message.mp3','m-r0-cannot_come_to_phone.mp3', 'm-r1-building.mp3',
'm-r2-cracking_walnuts.mp3', 'm-r3-polishing_monocole.mp3',
'm-r4-ripping_weights.mp3']
# list of all the mp3 file strings used in this code, found in chiServer

introPrompt = """
Hello, I am the old spice man. Read this in my voice. It sounds great,
doesn't it? What if I told you my voice could be your new answering machine?
Let's get started:
"""
optionHelloPrompt = """
How should I greet your caller?
1) Hello.
2) Hello callers.

"""
phoneNumPrompt = """
What is your phone number?
I can understand any of the following formats:
0123456789
012-345-6789
012.345.6789
(012)345-6789

Enter your phone number here:

"""
optionGenderPrompt = """
Are you a boy or a girl?
1) Boy
2) Girl

"""
optionMaleTaskPrompt = """
Now let's get creative. I'll tell them that you are too busy to
pick up the phone because you are:

1) ...ripping out mass loads of weights.
2) ...polishing their monacle smile.
3) ...cracking walnuts with their man mind.
4) ...building an orphanage for children with their bare hands while \
playing a sweet sweet lullaby for those children with two mallets against \
their ab xylophone.

Choose one or more options (e.g. "3" or "124")

"""
optionFemaleTaskPrompt = """
Now let's get creative. I'll tell them that you are too busy to
pick up the phone because you are:
1) ...riding a horse backwards with me
2) ...listening to me read romantic poetry while I make a \
bouquet of paper flowers from each red page.
3) ...ingesting my delicious old spice man spell.
4) ...being serenaded on the moon with a view of the earth while \
surviving off the oxygen in my lungs via a passionate kiss.
5) ...enjoying a delicious lobster dinner I prepared just for her while \
carrying her on my back safely through piranha infested waters.

Choose one or more options (e.g. "3" or "124")

"""
optionEndPrompt = """
Great. Now how should we end the call?

1) I'm on a horse.
2) I'm on a phone.
3) Swan dive!
4) This voicemail is now diamonds!
5) *old spice jingle*

Choose one or more options (e.g. "3" or "124")

"""
userConfirmedChoicePrompt = """
Are you happy with this voice message?
1) Yes, send me my voicemail file!
2) No, let's start over and try again.

"""
userFileNamePrompt = """
What would you like to name this voicemail file?
Note: Please include '.mp3' at the end of your filename.

"""
optionHello = ''
phoneNum = ''
optionGender = ''
optionMaleTask = ''
optionFemaleTask = ''
optionEnd = ''
userConfirmedChoice = ''
userFileName = ''
# global variables to store user's mp3 file choices.

def addmp3(mp3string):
    global outputFile
    global chiServer
    global myFile
    outputFile += mp3string+' '
# adds the new mp3 file to the queue of files to be combined at the end
    txtFileName = os.getcwd()
    txtFileName = os.path.join(txtFileName,userFileName[0:-3])
    txtFileName += 'txt'
# creates txtFileName, a string variable used to create a txt file listing
# the mp3 files used in the program
    myFile = open(txtFileName,'a')
    myFile.write(mp3string+'\n')
# creates myFile, a File object used to create the .txt file listing
# the mp3 files used in the program
    if sys.platform.startswith('win') == True:
        outputFile += '+'
# adds an additional character in the shell command for windows syntax
    mp3file = urllib2.urlopen(chiServer+mp3string)
# pulls the mp3 file from chiServer
    with open(mp3string,'wb') as output:
        output.write(mp3file.read())
# writes the files to the user's laptop

def ragequit(myString, chillPill):
    if myString != chillPill:
        print """
Incorrect # of command line arguments. oldspice.py accepts 11 arguments.
"""
        sys.exit()
# this function is used to end the program if the user does not type the
# command line arguments in the proper order specified by the .README

def ragequit2(myString, chillPill, argName):
    for i in myString:
        if i not in set(chillPill):
            print "Invalid entry for", argName,
            """
The following inputs are acceptable for :""",chillPill
            sys.exit()
# this function is used to end the program if the user does not enter a
# valid command line argument, specified by the .README

def hostage(demands, prompt):
    while True:
        myString = raw_input(prompt)
        for i in myString:
            if i not in set(demands):
                print """
I don't think that was a valid choice.
"""
                break
        else:
            break
    return myString
# this function is used during the interactive prompt, and stops the
# user from continuing on with the program until the user enters
# an input that satisfies the necessary criteria.

def interactivePrompt():
    global optionHello
    global phoneNum
    global optionGender
    global optionMaleTask
    global optionFemaleTask
    global optionEnd
    global userConfirmedChoice
    global userFileName
# defines global variables used in this function

    print introPrompt
# introduces the program to the user.

    optionHello = hostage('12', optionHelloPrompt)
# asks the user to input a preferred opening line, until the input is valid

    phoneNum = hostage('1234567890().-', phoneNumPrompt)
    # asks the user to input a phone number, until the input is valid
    phoneNum1 = phoneNum.replace('(','')
    phoneNum2 = phoneNum1.replace(')','')
    phoneNum3 = phoneNum2.replace('-','')
    phoneNum = phoneNum3.replace('.','')
# pulls out all of the undesired 'silent' aspects of the phone number.
    while len(phoneNum) != 10:
        print """
Try again: Please enter a 10 digit phone number this time.
    """
        phoneNum = hostage('1234567890().-', phoneNumPrompt)
        phoneNum1 = phoneNum.replace('(','')
        phoneNum2 = phoneNum1.replace(')','')
        phoneNum3 = phoneNum2.replace('-','')
        phoneNum = phoneNum3.replace('.','')
# prevents the user from continuing until a 10 digit phone number is entered.
    optionGender = hostage('12', optionGenderPrompt)
# asks the user to specify their gender, does not assume it.
    if optionGender == '1':
        optionMaleTask = hostage('1234', optionMaleTaskPrompt)
# asks a male user to input their reason they can't reach the phone until
# the input is valid.
    if optionGender == '2':
        optionFemaleTask = hostage('12345', optionFemaleTaskPrompt)
# asks a female user to input their reason they can't reach the phone until
# the input is valid.
    optionEnd = hostage('12345', optionEndPrompt)
#  allows the user to choose one or more options ending statements.

def recite():
# repeats the user's choices back to them.
    global optionHello
    global optionGender
    global optionMaleTask
    global optionGender
    global optionMaleTask
    global optionFemaleTask
    global optionEnd

    print """
Okay, here is what I have so far:

"""
#  begins to recite the user's choices back to them.

    if optionHello == '1':
        print 'Hello.',
    elif optionHello == '2':
        print 'Hello callers.',
#  returns the user's choice of greeting


    if optionGender == '1':
        print 'You have dialed', phoneNum+'.',

        print "The tall accomplished man you are calling can't come \
to the phone right now because they are",

        if '1' in optionMaleTask:
            print "ripping out mass loads of weights.",
        if '2' in optionMaleTask:
            print "polishing their monacle smile.",
        if '3' in optionMaleTask:
            print "cracking walnuts with their man mind.",
        if '4' in optionMaleTask:
            print "building an orphanage for children with their bare \
hands while playing a sweet sweet lullaby for those children with \
two mallets against their ab xylophone.",
# returns a male user's one or more reasons they
# can't answer

        print "But leave a message and they'll return your \
call as soon as possible.",

    elif optionGender == '2':
        print "The lovely, talented, intelligent, beautiful, sophisticated \
lady at""", phoneNum, "is unable to take your call. She's busy",
# prints this part of the script if the user is a girl

        if '1' in optionFemaleTask:
            print "riding a horse backwards with me,",
        if '2' in optionFemaleTask:
            print "listening to me read romantic poetry while I make \
a bouquet of paper flowers from each red page,",
        if '3' in optionFemaleTask:
            print "ingesting my delicious old spice man spell,",
        if '4' in optionFemaleTask:
            print "being serenaded on the moon with a view of the earth while \
surviving off the oxygen in my lungs via a passionate kiss,",
        if '5' in optionFemaleTask:
            print "enjoying a delicious lobster dinner I prepared just for \
her while carrying her on my back safely through piranha infested waters,",
# returns a female user's one or more reasons they can't answer, and repeats
# the prompt if the user input was invalid

        print "but she'll get back to you as soon as she can.",
# prints this part of the script if the user is a girl

    if '1' in optionEnd:
        print "I'm on a horse.",
    if '2' in optionEnd:
        print "I'm on a phone.",
    if '3' in optionEnd:
        print "Swan dive!",
    if '4' in optionEnd:
        print "This voicemail is now diamonds!",
    if '5' in optionEnd:
        print "*old spice jingle*",
# returns the user's one or more goodbye messages.


def makeVoicemail():
# function to make the mp3 file based on the user's choices
    global optionHello
    global optionGender
    global optionMaleTask
    global optionGender
    global optionMaleTask
    global optionFemaleTask
    global optionEnd
    global outputFile
    global chiServer

    if optionHello == '1':
        addmp3('a.mp3')
    elif optionHello == '2':
        addmp3('f-b1-hello_caller.mp3')
# adds the correct introductory mp3 file based on the user's choice

        for i in phoneNum:
            addmp3(str(i)+'.mp3')
# pulls the mp3 files of the user's phone number from the server

        addmp3('b.mp3')
# adds "the tall accomplished man you are calling can't come to the phone
# right now because they are" to the voicemail if the user is a boy

        if '1' in optionMaleTask:
            addmp3('c.mp3')
        if '2' in optionMaleTask:
            addmp3('p.mp3')
        if '3' in optionMaleTask:
            addmp3('e.mp3')
        if '4' in optionMaleTask:
            addmp3('d.mp3')
# adds a male user's choices for reasons they are busy to the mp3 file
        addmp3('f.mp3')
    elif optionGender == '2':
        addmp3('f-b2-lady_at.mp3')
# adds "the lovely, talented, intelligent, beautiful, sophisticated
# lady at" to the voicemail if the user is a girl
        for i in phoneNum:
            addmp3(str(i)+'.mp3')
# pulls the mp3 files of the user's phone number from the server
        addmp3('f-r0.2-she_is_busy.mp3')
# "she is busy" to the mp3 if the user is a girl
        if '1' in optionFemaleTask:
            addmp3('f-r5-riding_a_horse.mp3')
        if '2' in optionFemaleTask:
            addmp3('f-r2-listening_to_reading.mp3')
        if '3' in optionFemaleTask:
            addmp3('f-r1-ingesting_old_spice.mp3')
        if '4' in optionFemaleTask:
            addmp3('f-r4-moon_kiss.mp3')
        if '5' in optionFemaleTask:
            addmp3('f-r3-lobster_dinner.mp3')
# adds a female user's choices for reasons they are busy to the mp3 file
        addmp3('f-e1-she_will_get_back_to_you.mp3')
#adds "but she'll get back to you as soon as she can" if the user is a girl
    if '1' in optionEnd:
        addmp3('g.mp3')
    if '2' in optionEnd:
        addmp3('h.mp3')
    if '3' in optionEnd:
        addmp3('i.mp3')
    if '4' in optionEnd:
        addmp3('j.mp3')
    if '5' in optionEnd:
        addmp3('k.mp3')
# adds the user's requested ending, which is the same regardless of gender.

    myFile.close()
# closes the .txt file that outlines to the user the list of files used

if len(sys.argv) == 1:
    interactivePrompt()
    recite()
# prompts the user to go through the program interactively if they do not
# use command line arguments
    userConfirmedChoice = hostage('12',userConfirmedChoicePrompt)
    while userConfirmedChoice == '2':
        print """
Okay, we'll start over.

"""
        interactivePrompt()
        recite()
        userConfirmedChoice = hostage('12',userConfirmedChoicePrompt)

    userFileName = raw_input(userFileNamePrompt)
    while '.mp3' not in userFileName:
        print """
I don't think that was a valid choice.
Please include .mp3 at the end of your filename.
    """
        userFileName = raw_input(userFileNamePrompt)
# verififies that the user entered a valid filename

elif len(sys.argv) !=11:
    print """
Incorrect # of command line arguments. oldspice.py accepts 11 arguments.
"""
    sys.exit()
# verifies that the user entered the proper amount of command line arguments.

else:
    ragequit(sys.argv[1], '-g')
    ragequit(sys.argv[3], '-n')
    ragequit(sys.argv[5], '-r')
    ragequit(sys.argv[7], '-e')
    ragequit(sys.argv[9], '-o')
# stops the user from running the program without including the required flags.
    ragequit2(sys.argv[2], 'mf', 'sys.argv[2]')
    ragequit2(sys.argv[4], '1234567890().-', 'sys.argv[4]')
    ragequit2(sys.argv[6], '12345', 'sys.argv[6]')
    ragequit2(sys.argv[8], '12345', 'sys.argv[8]')
    if '.mp3' not in sys.argv[10]:
        print "Error: Please include .mp3 in your filename next time."
        sys.exit()
# checks to make sure that the user chose a filename with .mp3 at the end
    phoneNum = sys.argv[4]
    phoneNum1 = phoneNum.replace('(','')
    phoneNum2 = phoneNum1.replace(')','')
    phoneNum3 = phoneNum2.replace('-','')
    phoneNum = phoneNum3.replace('.','')
    if len(phoneNum) != 10:
        print "Error: Please enter a 10 digit phone number next time."
        sys.exit()
# checks to make sure the user chose a phone number with 10 digits
    optionHello = '1'
# adds hello to the mp3 file
    if sys.argv[2] == 'm':
        optionGender = '1'
        optionMaleTask = sys.argv[6]
        if '5' in sys.argv[6]:
            print "Error: There are only 4 arguments for a male voicemail."
            sys.exit()
# checks to make sure the user didn't put a 5th reason if he chose male
    elif sys.argv[2] == 'f':
        optionGender = '2'
        optionFemaleTask = sys.argv[6]
    phoneNum = sys.argv[4]
    optionEnd = sys.argv[8]
    userFileName = sys.argv[10]
# assigns the remaining variables to the command line arguments entered

if sys.platform.startswith('darwin') == True:
# checks to see if the user is operating on a mac
    outputFile = 'cat '
# initial preceding code for the command line code to produce the file
# ending code for the command line to save the file under the name
# requested by the user
    makeVoicemail()
# function to combine all the mp3 files
    outputFile += '> '+userFileName
    os.system(outputFile)
# adds the ending part of the command line code to save the file to the user's
# requested filename and creates the voicemail mp3 on the user's computer
    for i in mp3list:
        if os.path.isfile(i):
            os.system('rm '+i)
# iterates through all of the mp3 files that can be pulled from chiServer,
# deletes any that have been used from the user's computer,
# leaving the user with only their voicemail mp3 file

if sys.platform.startswith('win') == True:
# checks to see if the user is operating on a windows
    outputFile = 'copy /b '
# initial preceding code for the command line code to produce the file
# ending code for the command line to save the file under the name
# requested by the user
    makeVoicemail()
# function to combine all the mp3 files
    del(outputFile[-1])
# removes an extraneous '+' character from the shell script
    outputFile += os.getcwd()+userFileName
# adds the final line of code to save the merged mp3 files to the user's directory
    os.system(outputFile)
# save's the combined mp3 file under the user's requested filename
    myDir = os.getcwd()
    files = os.listdir(myDir)
    for i in mp3list:
        if i in files:
            os.remove(os.path.join(myDir,i))
# iterates through all of the mp3 files that can be pulled from chiServer,
# deletes any that have been used from the user's computer, thereby leaving
# the user with only their voicemail mp3 file
    os.startfile(os.path.join(myDir,userFileName))
# opens up the mp3 file
