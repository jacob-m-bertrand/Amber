# Amber - A NationStates Tool
Amber is a state-of-the-art <a href='nationstates.net'>NationStates</a> military data tool, allowing militaries to easily create customizable target lists. Amber was commissioned by The North Pacific Army in 2021 at the request of their then Minister of Defense.

NationStates is an online political simulator, more information about it's military gameplay can be found <a href='https://www.nationstates.net/page=dispatch/id=1007926'>here.</a>
# Settings
Most settings should be self-explanatory, however some are not. Below is a list of the non-obvious settings and what they do.

<b>Crazy Tagging Mode</b>
Enables crazy tagging mode, an algorithm designed by <a href='https://www.nationstates.net/nation=westinor'>Westinor</a>. It uses 9 randomly selected methods for tagging to throw off chasers.  A minimum of 6 soldiers is recommended.

<b>Regions Can/Should Have a Founder</b>
This pair of options allows you to decide whether or not you want targets to have founders or whether you care. By selecting ‘Regions Can Have a Founder’, founders won’t be considered when creating lists. With this option unticked, you can decide whether or not regions should have founders with the ‘Regions Need to Have a Founder’ option.

<b>Enable AutoFind</b>
AutoFind is an algorithm in which the user gives a region (usually the jump point) and seperation, and Amber automatically finds as many targets as it can before that region with the given separation.

<b>Enable AutoHit</b>
AutoHit automatically inserts a target that must be hit during the operation. For example, if a pile must be hit to maintain endorsements. 

# Usage
To use Amber, generate a spyglass list and copy columns A-H. Paste those columns into the 'Input' box. Change any settings you want, then click 'Start Target Finding'. The process may take several minutes to complete, but on average is finished in around 30-60 seconds.

# Advanced Tag Checking
A core functionality of Amber is to check the factbooks of regions for certain keywords to check if the region is tagged. A list of these tags is found in ‘factbook_checks.txt’, which is included in this directory. Each keyword goes on its own line.
Amber can also check for tags to avoid, for example NPA tags. These tags should never appear in target lists, and keywords that are used to search for them are found in ‘factbook_avoids.txt’, with a keyword on each line.
