#!/usr/bin/env python
'''
Tape Sprocket Creator (insert more clever title here)

This script generates DXF outlines for feeder sprockets for e.g. perforated tape or film advance.

Copyright (C) 2011 Tim Gipson (drmn4ea at google mail)

Includes DXF generation code from Inkscape (inkscape.org) DXF output plugin:
- Copyright (C) 2005,2007 Aaron Spike, aaron@ekips.org
- template dxf_outlines.dxf added Feb 2008 by Alvin Penner, penner@vaxxine.com
- layers, transformation, flattening added April 2008 by Bob Cook, bob@bobcookdev.com
- improved layering support added Feb 2009 by T. R. Gipson, drmn4ea at google mail

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
'''

import math
import Tkinter as Tk
from tkFileDialog import asksaveasfilename
import dxf_templates_b2


def p2r(r, theta):
    #python's builtin math functions work in radians, so convert...
    theta = theta * (math.pi / 180.0)
    return r*math.cos(theta), r*math.sin(theta)


def dist(xy1, xy2):
    dist_x = abs(xy1[0] - xy2[0])
    dist_y = abs(xy1[1] - xy2[1])
    distance = math.sqrt((dist_x*dist_x) + (dist_y*dist_y))
    return distance


def compute_gear():
    global rect_lines
    global rect_comp_drills

    # grab the variables from text boxes
    n_teeth = float(nTeethValue.get())
    tooth_dia = float(toothDiaValue.get())
    tooth_pitch = float(toothPitchValue.get())
    flank_height = float(toothFlankValue.get())
    tooth_length_pct = float(toothHeightValue.get())

#Process:
# Generate a single tooth: taper edges so that (leftedge-to-rightedge) distance is no greater than the outer-to-outer distance at the base of the tooth: 'undo' the splay caused by the teeth being on an arc.
# This splay is given by the angle that subtends the arc bounded by the outer edges of the tooth (straight part at top of "design" radius). This is the center-to-center arc length + the arc 'inside' one tooth (1/2 tooth on the outside of each center).
# But we already know the *angle* formed by the center-to-center tooth pitch: (360/nTeeth), so we only need to find the arc length inside one tooth and its resulting angle, and add this to the pitch angle.

# Intratooth arc angle in degrees: (360/pi) * asin(C / 2r), where C = chord length.

# So the outer (as well as inner) surface of each tooth must come in 1/2 that amount (relative to radial / sticking straight out).



# Generate important radii: what I'm calling "design" radius (top of tooth flank; widest point of "where the tape sits"). For thin tapes this is approximately the same as inner radius, but we may as well go ahead and calculate it.
# Inner radius
# Maximum outer radius. This is where the tooth comes to a point at the given face angle. The final outer radius can be chosen anywhere between design radius and maximum outer radius (tops of teeth will be cut off).

# Design radius: ((nTeeth*pitch) / (2*pi))
# Inner radius: ((nTeeth*pitch) / (2*pi)) - flank height

# Outer radius: design radius + tooth face height
# Tooth inner angle is 90 - outer angle, so, tooth face height: tan(90-angle) = h / (w/2) --> h = (w/2)*tan(90-angle)

####FIXME: Tooth auto-angle currently broken. Making two adjacent teeth 'straight' on their outside edges is not enough!

# Calculate where the nominal teeth intersect these circles and generate lines between them.


    design_circumference = n_teeth * tooth_pitch
    design_radius = design_circumference / (2.0*math.pi)
    inner_radius = design_radius - flank_height


# get the allowed range for the outer radius
# to do this, need to get (or suggest) the tooth angle. This will be related to 'splay' described above.
# Unless the user has chosen (or overridden) a desired angle, calculate and use the recommended value


    chord_angle = 2.0 * (180.0/math.pi) * math.asin(tooth_dia / (2.0 * design_radius))

    try:
        tooth_outer_angle = float(toothFaceAngleValue.get())
        tooth_inner_angle = 90.0 - tooth_outer_angle - (chord_angle / 2.0)
    except ValueError:
        tooth_outer_angle = (chord_angle + (360.0 / n_teeth)) / 4.0 # half the equidistant-to-two-teeth to edge-of-tooth angle -> 1/4 of edge-to-edge angle
        tooth_inner_angle = 90.0 - tooth_outer_angle - (chord_angle / 2.0)
        toothFaceAngleValue.set(str(tooth_outer_angle))

    print "Tooth centers angle: %.2f" % (360.0/n_teeth)
    print "Tooth width contribution: %2f" % chord_angle
    print "Tooth edges angle: %.2f" % ((360.0/n_teeth) + chord_angle)


    print "Design circumference = %.2f radius = %.2f" % (design_circumference, design_radius)
    print "Min. tooth angle: %.2f degrees from vertical" % tooth_outer_angle


    tooth_face_height = (tooth_dia / 2.0) * math.tan(tooth_inner_angle * (math.pi / 180.0))

    max_outer_radius = design_radius + tooth_face_height


    user_outer_radius = design_radius + ((max_outer_radius - design_radius) * (tooth_length_pct/100.0)) #design_radius + (design_radius*0.5) #max_outer_radius

    print "Resulting tooth height (angled portion) = %.2f" % tooth_face_height
    print "Outer radius = %.2f" % max_outer_radius


# From list of tooth centers...
# Flank (starts,ends): tooth center +/- tooth chordal angle
# Faces: Remember, working in polar coords. Doublecheck this, but I think where the angle of the ray intersecting the "end of face" (between 0 and chord_angle/2 relative to tooth center) falls will be proportional to where end of face falls between design radius and max. outer radius.

    face_ratio = (user_outer_radius - design_radius) / (max_outer_radius - design_radius) # will produce a result between 0 and 1

# Actual angle will be the inverse of this, since where user outer = max outer, angle is 0.

    tooth_ending_angle = (1 - face_ratio) * (chord_angle / 2)

    tooth_centers = []

    for i in range(0, int(n_teeth)):
        tooth_centers.append((360.0 / n_teeth) * i)

# For each tooth:
#   line(start of flank ~ end of flank)
#   line(end of flank (start of face) ~ where face intersects outer radius)
#   line/arc(first ~ second face intersection with outer radius)
#   reverse of the above (end and start of face, flank)
#   arc(end of flank ~ start of next tooth's flank)

# pairs of (r,theta) for line starting and ending point
    polar_lines = []

    for i in tooth_centers:
    # Polar angles are usually considered increasing counterclockwise, with zero "to the right" - we'll do the same,
    # going from righthand side to lefthand side of each tooth.
    # Right flank
        polar_lines.append([ [inner_radius, i-(chord_angle/2)], [design_radius, i-(chord_angle/2)] ])
    # Right face
        polar_lines.append([ [design_radius, i-(chord_angle/2)], [user_outer_radius, i-tooth_ending_angle] ])
    # blunted tip
        polar_lines.append([ [user_outer_radius, i-tooth_ending_angle], [user_outer_radius, i+tooth_ending_angle] ])
    # left face
        polar_lines.append([ [user_outer_radius, i+tooth_ending_angle], [design_radius, i+(chord_angle/2)] ])
    # left flank
        polar_lines.append([ [design_radius, i+(chord_angle/2)], [inner_radius, i+(chord_angle/2)] ])
    # finally, the space between this and the next tooth
        polar_lines.append([ [inner_radius, i+(chord_angle/2)], [inner_radius, i+(360.0/n_teeth)-(chord_angle/2)] ])


    # if cutter crap-removal on: remove the shoulder left behind when outside-pocketing concave portions; in this case where the tooth flanks meet the inner radius.
    # cut (d/2) below the radius, then across by at least d (+ ballhair?) to ensure pocket algorithm will allow cutting in there, then back to inner radius.
    # It is an error if d exceeds the distance between teeth.

    # Hmm... will also fail if d exceeds 1/2 distance between teeth, and makes more math for me. For now just add drills that will take the shoulders off.

    polar_comp_drills = []

    try:
        comp_r = float(cutterDiaLabelValue.get()) / 2.0
        for i in tooth_centers:
            # at end of this tooth
            # put the hole center @ inside diameter line, 1 cutter radius from edge of tooth
            polar_comp_drills.append([inner_radius, i+(chord_angle/2) + (360*comp_r/(2*math.pi*(inner_radius - comp_r)))])
            # at start of next tooth
            polar_comp_drills.append([inner_radius , i+(360.0/n_teeth)-(chord_angle/2) - (360*comp_r/(2*math.pi*(inner_radius - comp_r)))])
    except ValueError:
        comp_r = 0



#print polar_lines

    rect_lines = []
    rect_comp_drills = []

#print polar_lines[0][0][0]

    for i in polar_lines:
        rect_lines.append([p2r(i[0][0],i[0][1]),  p2r(i[1][0],i[1][1])])

    for i in polar_comp_drills:
        rect_comp_drills.append(p2r(i[0],i[1]))

#print rect_lines


# based on the outer radius, scale the drawing to cover about 75% of the window
    k = ((min(canvas_width, canvas_height)+0.0) / (user_outer_radius*2.0)) * 0.75
    print "k=%.2f" % k
#print polar_lines[2][1][0]


    drawcanvas.delete(Tk.ALL)

    canvas_hcenter = canvas_width / 2
    canvas_vcenter = canvas_height / 2

    for i in rect_lines:
        drawcanvas.create_line(i[0][0]*k + canvas_vcenter, i[0][1]*k + canvas_hcenter, i[1][0]*k + canvas_vcenter, i[1][1]*k + canvas_hcenter )
    # if cutter compensation, show drills too
    for i in rect_comp_drills:
        drawcanvas.create_oval(i[0]*k + canvas_vcenter - .01, i[1]*k + canvas_hcenter - .01, i[0]*k + canvas_vcenter + .01, i[1]*k + canvas_hcenter + .01)


# distance between line 1 (second coord) and line 7 (second coord) should = distance between a given edge (same lines 1st coord). Both should = pitch.
    print "Check: Tooth edge-to-edge distance (should equal pitch+tooth_dia, or %.2f): at bottom %.2f, at top %.2f" % (tooth_pitch + tooth_dia, dist(rect_lines[1][0], rect_lines[9][1]), dist(rect_lines[1][1], rect_lines[9][0]))

# show we are at least calculating distance between the right points...
# bottom of tooth
    drawcanvas.create_line(rect_lines[1][0][0]*k + canvas_vcenter, rect_lines[1][0][1]*k + canvas_hcenter, rect_lines[9][1][0]*k + canvas_vcenter, rect_lines[9][1][1]*k + canvas_hcenter, fill="green")
# top of tooth
    drawcanvas.create_line(rect_lines[1][1][0]*k + canvas_vcenter, rect_lines[1][1][1]*k + canvas_hcenter, rect_lines[9][0][0]*k + canvas_vcenter, rect_lines[9][0][1]*k + canvas_hcenter, fill="green")

    InnerDiaLabelText.set("Inner diameter: %.3f" % (inner_radius * 2))
    DesignDiaLabelText.set("Design diameter: %.3f" % (design_radius * 2))
    OuterDiaLabelText.set("Outer diameter: %.3f" % (user_outer_radius * 2))
    MaxOuterDiaLabelText.set("Max. outer diameter: %.3f" % (max_outer_radius * 2))
    ToothEdgesLabelText.set("Tooth edge-to-edge distances (should be < %.2f, pitch + tooth dia):\n bottom %.2f, tip %.2f" % (tooth_pitch + tooth_dia, dist(rect_lines[1][0], rect_lines[9][1]), dist(rect_lines[1][1], rect_lines[9][0])) )



def dxf_add(str):
    global dxf
    dxf += str

def dxf_insert_code(code, value):
    global dxf
    dxf += code + "\n" + value + "\n"

def dxf_line(theline, scale):
    global dxf
    dxf_insert_code(   '0', 'LINE' )
    dxf_insert_code(   '8', 'Default' ) # layer name
    dxf_insert_code(  '62', '4' )
    dxf_insert_code(   '5', '255' ) # DXF entity handle
    dxf_insert_code( '100', 'AcDbEntity' )
    dxf_insert_code( '100', 'AcDbLine' )
    dxf_insert_code(  '10', '%f' % (theline[0][0] * scale) ) # line start x
    dxf_insert_code(  '20', '%f' % (theline[0][1] * scale) ) # line start y
    dxf_insert_code(  '30', '0.0' )
    dxf_insert_code(  '11', '%f' % (theline[1][0] * scale) ) # line end x
    dxf_insert_code(  '21', '%f' % (theline[1][1] * scale) ) # line end x
    dxf_insert_code(  '31', '0.0' )

def dxf_point(point, scale):
    global dxf
    dxf_insert_code(   '0', 'POINT' )
    dxf_insert_code(   '8', 'Drills' ) # layer name
    dxf_insert_code(  '62', '4' )
    dxf_insert_code(   '5', '255' ) # DXF entity handle
    dxf_insert_code( '100', 'AcDbEntity' )
    dxf_insert_code( '100', 'AcDbPoint' )
    dxf_insert_code(  '10', '%f' % (point[0] * scale) )
    dxf_insert_code(  '20', '%f' % (point[1] * scale) )
    dxf_insert_code(  '30', '0.0' )

def dxf_layer_table(layers):
    global dxf
    dxf_insert_code('0', 'TABLE')
    dxf_insert_code('2', 'LAYER')
    dxf_insert_code('5', '2')
    dxf_insert_code('330', '0')
    dxf_insert_code('100', 'AcDbSymbolTable')
    # group code 70 tells a reader how many table records to expect (e.g. pre-allocate memory for).
    # It must be greater or equal to the actual number of records
    dxf_insert_code('70',str(len(layers)))

    for layer in layers:
         dxf_insert_code('0', 'LAYER')
         dxf_insert_code('5', '10')
         dxf_insert_code('330', '2')
         dxf_insert_code('100', 'AcDbSymbolTableRecord')
         dxf_insert_code('100', 'AcDbLayerTableRecord')
         dxf_insert_code('2', layer)
         dxf_insert_code('70', '0')
         dxf_insert_code('62', '7')
         dxf_insert_code('6', 'CONTINUOUS')

    dxf_insert_code('0','ENDTAB')
    dxf_insert_code('0','ENDSEC')




def write_dxf():
    global rect_lines
    global rect_comp_drills
    global dxf
    # handle unit scaling if any
    unit_scale = unitsValue.get()
    dxf = ""

    # headery crap
    dxf_insert_code( '999', 'Tape sprocket created by tapegear.py' ) # bogus code 'almost' universally accepted as a comment; comment this line if your DXF parser complains
    dxf_add( dxf_templates_b2.r14_header )
    dxf_layer_table(["Default", "Drills"])
    dxf_add( dxf_templates_b2.r14_blocks )
    # now the actual geometry...
    for i in rect_lines:
        dxf_line(i, unit_scale)

    for i in rect_comp_drills:
        dxf_point(i, unit_scale)

    # and footer
    dxf_add( dxf_templates_b2.r14_footer )
    filename = asksaveasfilename(filetypes=[('DXF (Drawing eXchange Format r14)', '.dxf')], initialfile='sprocket.dxf')
    if filename != None:
        print "Started writing DXF"
        f = open(filename, 'w')
        f.write(dxf)
        f.close()
        print "Finished writing DXF"



# globals
rect_lines = []
rect_comp_drills = []
dxf = ""


canvas_width = 400
canvas_height = 400

k=8

root = Tk.Tk()


root.wm_title("Tape Sprocket Creator")


# create GUI widgetyness
# Warning, extreme ugliness ahead...
drawcontainer = Tk.Frame(root, width=canvas_width, height=canvas_height, bg="", colormap="new")
drawcanvas = Tk.Canvas(drawcontainer, width=canvas_width, height=canvas_height)
drawcanvas.pack(side=Tk.LEFT, fill=Tk.BOTH, expand=1)
drawcontainer.pack(side=Tk.LEFT)

ctrlcontainer = Tk.Frame(root)


basicsLFrame = Tk.LabelFrame(ctrlcontainer, text="Basic parameters") # Labeled frame for the basics (# teeth, etc.)

nTeethFrame = Tk.Frame(basicsLFrame) # Container for nTeeth (label, value)
nTeethLabel = Tk.Label(nTeethFrame, text="# of teeth:")
nTeethLabel.pack(side=Tk.LEFT)
nTeethValue = Tk.StringVar()
nTeethEntry = Tk.Entry(nTeethFrame, textvariable=nTeethValue)
nTeethEntry.pack(side=Tk.RIGHT) # pack next to label
nTeethFrame.pack(side=Tk.TOP)

toothDiaFrame = Tk.Frame(basicsLFrame) # Container for toothDia (label, value)
toothDiaLabel = Tk.Label(toothDiaFrame, text="Tooth width/diameter:")
toothDiaLabel.pack(side=Tk.LEFT)
toothDiaValue = Tk.StringVar()
toothDiaEntry = Tk.Entry(toothDiaFrame, textvariable=toothDiaValue)
toothDiaEntry.pack(side=Tk.RIGHT) # pack next to label
toothDiaFrame.pack(side=Tk.TOP)

toothPitchFrame = Tk.Frame(basicsLFrame) # Container for toothPitch (label, value)
toothPitchLabel = Tk.Label(toothPitchFrame, text="Tooth spacing (pitch):")
toothPitchLabel.pack(side=Tk.LEFT)
toothPitchValue = Tk.StringVar()
toothPitchEntry = Tk.Entry(toothPitchFrame, textvariable=toothPitchValue)
toothPitchEntry.pack(side=Tk.RIGHT) # pack next to label
toothPitchFrame.pack(side=Tk.TOP)

toothFlankFrame = Tk.Frame(basicsLFrame) # Container for toothFlank (label, value)
toothFlankLabel = Tk.Label(toothFlankFrame, text="Tooth flank height:")
toothFlankLabel.pack(side=Tk.LEFT)
toothFlankValue = Tk.StringVar()
toothFlankEntry = Tk.Entry(toothFlankFrame, textvariable=toothFlankValue)
toothFlankEntry.pack(side=Tk.RIGHT) # pack next to label
toothFlankFrame.pack(side=Tk.TOP)

toothHeightFrame = Tk.Frame(basicsLFrame) # Container for toothHeight (label, value)
toothHeightLabel = Tk.Label(toothHeightFrame, text="Tooth length (%):")
toothHeightLabel.pack(side=Tk.LEFT)
toothHeightValue = Tk.StringVar()
toothHeightEntry = Tk.Entry(toothHeightFrame, textvariable=toothHeightValue)
toothHeightEntry.pack(side=Tk.RIGHT) # pack next to label
toothHeightFrame.pack(side=Tk.TOP)


basicsLFrame.pack(side=Tk.TOP)

assistLFrame = Tk.LabelFrame(ctrlcontainer, text="Auto-calculated parameters") # Labeled frame for auto-suggested values

toothFaceAngleFrame = Tk.Frame(assistLFrame) # Container for toothFaceAngle (label, value)
toothFaceAngleLabel = Tk.Label(toothFaceAngleFrame, text="Tooth face angle:")
toothFaceAngleLabel.pack(side=Tk.LEFT)
toothFaceAngleValue = Tk.StringVar()
toothFaceAngleEntry = Tk.Entry(toothFaceAngleFrame, textvariable=toothFaceAngleValue)
toothFaceAngleEntry.pack(side=Tk.RIGHT) # pack next to label
toothFaceAngleFrame.pack(side=Tk.TOP)



assistLFrame.pack(side=Tk.TOP)

# UI labels for the diameters and stuff

diaFrame = Tk.Frame(ctrlcontainer)

InnerDiaLabelText = Tk.StringVar()
InnerDiaLabelText.set("Inner diameter:")
InnerDiaLabel = Tk.Label(ctrlcontainer, textvariable=InnerDiaLabelText)
DesignDiaLabelText = Tk.StringVar()
DesignDiaLabelText.set("Design diameter:")
DesignDiaLabel = Tk.Label(ctrlcontainer, textvariable=DesignDiaLabelText)
OuterDiaLabelText = Tk.StringVar()
OuterDiaLabelText.set("Outer diameter:")
OuterDiaLabel = Tk.Label(ctrlcontainer, textvariable=OuterDiaLabelText)
MaxOuterDiaLabelText = Tk.StringVar()
MaxOuterDiaLabelText.set("Max. outer diameter:")
MaxOuterDiaLabel = Tk.Label(ctrlcontainer, textvariable=MaxOuterDiaLabelText)
ToothEdgesLabelText = Tk.StringVar()
ToothEdgesLabelText.set("Tooth edge-to-edge distance:")
ToothEdgesLabel = Tk.Label(ctrlcontainer, textvariable=ToothEdgesLabelText)



InnerDiaLabel.pack(side=Tk.TOP, fill=Tk.X)
DesignDiaLabel.pack(side=Tk.TOP, fill=Tk.X)
OuterDiaLabel.pack(side=Tk.TOP, fill=Tk.X)
MaxOuterDiaLabel.pack(side=Tk.TOP, fill=Tk.X)
ToothEdgesLabel.pack(side=Tk.TOP, fill=Tk.X)

diaFrame.pack(side=Tk.TOP, fill=Tk.X)

# Buttons! And DXF-writing options

gobutton = Tk.Button(ctrlcontainer, text="Compute / Auto angle", command=compute_gear)
gobutton.pack(side=Tk.TOP, fill=Tk.X)


# Holder for cutter radius removal controls
cutterDiaLabelFrame = Tk.LabelFrame(ctrlcontainer, text="Cutter radius leftovers removal (CNC mills)")

RadiusCompensationValue = Tk.BooleanVar()
cbRadiusCompensation = Tk.Checkbutton(cutterDiaLabelFrame, text="Remove cutter leftovers", variable=RadiusCompensationValue, onvalue=True, offvalue=False)
cbRadiusCompensation.pack(side=Tk.TOP)

cutterDiaLabel = Tk.Label(cutterDiaLabelFrame, text="Cutter diameter:")
cutterDiaLabel.pack(side=Tk.LEFT)
cutterDiaLabelValue = Tk.StringVar()
cutterDiaLabelEntry = Tk.Entry(cutterDiaLabelFrame, textvariable=cutterDiaLabelValue)
cutterDiaLabelEntry.pack(side=Tk.RIGHT) # pack next to label

cutterDiaLabelFrame.pack(side=Tk.TOP)


unitsLabelFrame = Tk.LabelFrame(ctrlcontainer, text="Unit conversion")
unitsValue = Tk.DoubleVar()
unitsValue.set(1.0) # default to no conversion
unitsNoneBtn = Tk.Radiobutton(unitsLabelFrame, text="None", variable=unitsValue, value=1.0).pack(side=Tk.TOP)
unitsMmBtn = Tk.Radiobutton(unitsLabelFrame, text="inches to mm", variable=unitsValue, value=25.4).pack(side=Tk.TOP)
unitsInchBtn = Tk.Radiobutton(unitsLabelFrame, text="mm to inches", variable=unitsValue, value=0.0393700787).pack(side=Tk.TOP)
unitsLabelFrame.pack(side=Tk.TOP)




savebutton = Tk.Button(ctrlcontainer, text="Write DXF", command=write_dxf)
savebutton.pack(side=Tk.TOP, fill=Tk.X)

ctrlcontainer.pack(side=Tk.LEFT)



Tk.mainloop()

# Tkinter root.quit does NOT stop the Tcl interpreter and destroy all widgets (as claimed by the docs), only quits the main event loop. The window and widgets must be
# destroyed with root.destroy() after to avoid the dreaded 'NULL tstate' Python crash. http://mail.python.org/pipermail/python-bugs-list/2002-November/014207.html
# BUT... clicking the little 'x' does do both, so destroy *itself* will throw an error in this case. So, some ugly...

try:
    root.destroy()
except:
    pass
