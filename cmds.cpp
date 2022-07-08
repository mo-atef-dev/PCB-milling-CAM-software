#include "cmds.h"

struct StepCommand
{
     int x;
     int y;
     int z;
};

static int sign(int x)
{
    if(x > 0)
        return 1;
    else if(x < 0)
        return -1;
    else
        return 0;
}

static unsigned char signBit(int x)
{
    if(x < 0)
        return 1u;
    else
        return 0;
}

static std::vector<StepCommand> DivideCommand(StepCommand cmd)
{
    using namespace std;

    vector<StepCommand> div;

    // Determine the maximum and secondary axis
    int maxAxis, secAxis, zAxis;

    zAxis = abs(cmd.z);

    if(abs(cmd.x) > abs(cmd.y))
    {
        maxAxis = abs(cmd.x);
        secAxis = abs(cmd.y);
    }
    else
    {
        maxAxis = abs(cmd.y);
        secAxis = abs(cmd.x);
    }

    // In case there is only motion in Z
    if(maxAxis == 0)
    {
        while(zAxis > 255)
        {
            div.push_back({0, 0, 255*sign(cmd.z)});
            zAxis -= 255;
        }
        if(zAxis > 0)
        {
            div.push_back({0, 0, zAxis*sign(cmd.z)});
        }
        return div;
    }

    /// Variables for the calculations

    int countFull = maxAxis/255;                // Number of commands with max axis at 255
    int count = countFull + (maxAxis%255 != 0); // Total number of commands needed for the motion
    int countZ = cmd.z/255;                     // Number of commands needed for Z axis (currently not used)

    // Divisions of the secondary, Z, and max axis
    int maxAxisFinDiv = maxAxis - countFull*255;
    int secAxisDiv = 255*secAxis/maxAxis;
    int secAxisFinDiv = maxAxisFinDiv*secAxis/maxAxis;
    int zAxisFinDiv = zAxis%255;

    // Number of remaining steps lost due to division
    int missingSteps = secAxis - (secAxisDiv*countFull + secAxisFinDiv);

    /// First pass (divide the command initially with some error in final position)

    // Add the full commands
    for(int i = 0; i < countFull; i++)
    {
        // Add the z part
        int addZ = 0;
        if(zAxis > 255)
        {
            addZ = 255*sign(cmd.z);
            zAxis -= 255;
        }
        else if(zAxis > 0)
        {
            addZ = zAxis*sign(cmd.z);
        }

        if(abs(cmd.x) > abs(cmd.y))
            div.push_back({255*sign(cmd.x), secAxisDiv*sign(cmd.y), addZ});
        else
            div.push_back({secAxisDiv*sign(cmd.x), 255*sign(cmd.y), addZ});
    }

    // Add the final command if needed
    if((maxAxis%255 != 0))
    {
        if(abs(cmd.x) > abs(cmd.y))
            div.push_back({maxAxisFinDiv*sign(cmd.x), secAxisFinDiv*sign(cmd.y), 0});
        else
            div.push_back({secAxisFinDiv*sign(cmd.x), maxAxisFinDiv*sign(cmd.y), 0});
    }

    /// Second pass (add any missing steps)
    for(int i = 0; i < missingSteps; i++)
    {
        if(abs(cmd.x) > abs(cmd.y))
            div[i].y += sign(cmd.y);
        else
            div[i].x += sign(cmd.x);
    }

    // Add missing Z steps
    while(zAxis > 0)
    {
        // Add the z part
        div.push_back({0, 0, 255*sign(cmd.z)});
        zAxis -= 255;
    }
    if(zAxis > 0)
    {
        div.push_back({0, 0, zAxis*sign(cmd.z)});
    }

    /// Third pass (check the total number of steps in each axis adds up to the original command

    int sumx = 0;
    int sumy = 0;
    int sumz = 0;

    for(int i = 0; i < div.size(); i++)
    {
        sumx += div[i].x;
        sumy += div[i].y;
        sumz += div[i].z;
    }

    if(sumx != cmd.x || sumy != cmd.y || sumz != cmd.z)
    {
        // Additional debugging information can be added here
        cout << "## Critical error! sum of divided commands don't add up to the original command ##\n";
    }

    return div;
}

static StepCommand subCommands(StepCommand c1, StepCommand c2)
{
    StepCommand c3;
    c3.x = c1.x - c2.x;
    c3.y = c1.y - c2.y;
    c3.z = c1.z - c2.z;
    return c3;
}

static StepCommand addCommands(StepCommand c1, StepCommand c2)
{
    StepCommand c3;
    c3.x = c1.x + c2.x;
    c3.y = c1.y + c2.y;
    c3.z = c1.z + c2.z;
    return c3;
}

std::vector<OutCommand> step_mov(std::vector <CompressedCommand> cmds, int xStepPermm, int yStepPermm, int zStepPermm)
{
    using namespace std;
    vector<StepCommand> divided;

    // Convert the mm commands to absolute commands in units of motor steps
    vector<StepCommand> abso(cmds.size());

    // Debug section
    cout << "Starting to convert commands\n" << "Steps per mm: " << xStepPermm << "\n";
    // End of debug section

    for(int i = 0; i < cmds.size(); i++)
    {
        abso[i].x = cmds[i].x * xStepPermm;
        abso[i].y = cmds[i].y * yStepPermm;
        abso[i].z = cmds[i].z * zStepPermm;

        // Debug section
        cout << "Absolute step coordinates x: " << abso[i].x << ", y: " << abso[i].y << ", z: " << abso[i].z << "\n";
        // End of debug section
    }

    // Calculate the relative command list (each command in this list is the difference between two consecutive commands in the absolute list)
    vector<StepCommand> rel;

    // Initial command from 0, 0, 0
    rel.push_back(subCommands(abso[0], {0, 0, 0}));

    for(int i = 1; i < abso.size(); i++)
    {
        rel.push_back(subCommands(abso[i], abso[i-1]));

        // Debug section
        cout << "Relative commands x: " << rel[i-1].x << ", y: " << rel[i-1].y << ", z: " << rel[i-1].z << "\n";
        // End of debug section
    }

    // Divide the commands to max 255 steps in any of the 3 directions
    for(int i = 0; i < rel.size(); i++)
    {
        vector<StepCommand> added = DivideCommand(rel[i]);
        divided.insert(divided.end(), added.begin(), added.end());
    }

    // Debug section
    for(int i = 0; i < divided.size(); i++)
    {
        cout << "Divided commands x: " << divided[i].x << ", y: " << divided[i].y << ", z: " << divided[i].z << "\n";
    }
    // End of debug section

    // Output vector
    vector<OutCommand> out(divided.size());

    // Convert the StepCommand to OutCommand
    for(int i = 0; i < divided.size(); i++)
    {
        out[i].x = abs(divided[i].x);
        out[i].y = abs(divided[i].y);
        out[i].z = abs(divided[i].z);

        out[i].acc = 0x01ul; // Constant speed 0x01
        out[i].acc = out[i].acc | (signBit(divided[i].x) << 7) | (signBit(divided[i].y) << 6) | (signBit(divided[i].z) << 5);

        // Debug section
        // The + operator before the out[i].x is used to print x in a number format (without it it will be printed as a character)
        cout << "Final output commands";
        printf(" x: %hhu, y: %hhu, z: %hhu", out[i].x, out[i].y, out[i].z);
        cout << ", directions: " << (int)((out[i].acc & 0x80) >> 7) << (int)((out[i].acc & 0x40) >> 6) << (int)((out[i].acc & 0x20) >> 5) << ", speeds: " << (int)(out[i].acc & 0x1F) << "\n";
        // End of debug section
    }

    return out;
}
