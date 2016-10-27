<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="7.5.0">
<drawing>
<settings>
<setting alwaysvectorfont="yes"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="inch" style="lines" multiple="1" display="no" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="1" visible="no" active="no"/>
<layer number="16" name="Bottom" color="1" fill="1" visible="no" active="no"/>
<layer number="17" name="Pads" color="2" fill="1" visible="no" active="no"/>
<layer number="18" name="Vias" color="2" fill="1" visible="no" active="no"/>
<layer number="19" name="Unrouted" color="6" fill="1" visible="no" active="no"/>
<layer number="20" name="Dimension" color="15" fill="1" visible="no" active="no"/>
<layer number="21" name="tPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="22" name="bPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="23" name="tOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="24" name="bOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="25" name="tNames" color="7" fill="1" visible="no" active="no"/>
<layer number="26" name="bNames" color="7" fill="1" visible="no" active="no"/>
<layer number="27" name="tValues" color="7" fill="1" visible="no" active="no"/>
<layer number="28" name="bValues" color="7" fill="1" visible="no" active="no"/>
<layer number="29" name="tStop" color="7" fill="3" visible="no" active="no"/>
<layer number="30" name="bStop" color="7" fill="6" visible="no" active="no"/>
<layer number="31" name="tCream" color="7" fill="4" visible="no" active="no"/>
<layer number="32" name="bCream" color="7" fill="5" visible="no" active="no"/>
<layer number="33" name="tFinish" color="6" fill="3" visible="no" active="no"/>
<layer number="34" name="bFinish" color="6" fill="6" visible="no" active="no"/>
<layer number="35" name="tGlue" color="7" fill="4" visible="no" active="no"/>
<layer number="36" name="bGlue" color="7" fill="5" visible="no" active="no"/>
<layer number="37" name="tTest" color="7" fill="1" visible="no" active="no"/>
<layer number="38" name="bTest" color="7" fill="1" visible="no" active="no"/>
<layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
<layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
<layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
<layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
<layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
<layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
<layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
<layer number="46" name="Milling" color="3" fill="1" visible="no" active="no"/>
<layer number="47" name="Measures" color="7" fill="1" visible="no" active="no"/>
<layer number="48" name="Document" color="7" fill="1" visible="no" active="no"/>
<layer number="49" name="Reference" color="7" fill="1" visible="no" active="no"/>
<layer number="51" name="tDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="90" name="Modules" color="5" fill="1" visible="yes" active="yes"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
<layer number="99" name="SpiceOrder" color="5" fill="1" visible="yes" active="yes"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="special">
<description>&lt;b&gt;Special Devices&lt;/b&gt;&lt;p&gt;
7-segment displays, switches, heatsinks, crystals, transformers, etc.&lt;p&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
<package name="7SEG-13">
<description>7-segment &lt;B&gt;DISPLAY&lt;/B&gt;&lt;p&gt;
13 mm</description>
<wire x1="6.096" y1="-8.636" x2="6.096" y2="8.636" width="0.1524" layer="21"/>
<wire x1="-6.096" y1="8.636" x2="6.096" y2="8.636" width="0.1524" layer="21"/>
<wire x1="-6.096" y1="8.636" x2="-6.096" y2="-8.636" width="0.1524" layer="21"/>
<wire x1="6.096" y1="-8.636" x2="-6.096" y2="-8.636" width="0.1524" layer="21"/>
<wire x1="3.5306" y1="4.7498" x2="4.4196" y2="5.461" width="0.254" layer="21"/>
<wire x1="4.4196" y1="5.461" x2="4.8768" y2="5.1308" width="0.254" layer="21"/>
<wire x1="2.8448" y1="1.0414" x2="3.9878" y2="0.4064" width="0.254" layer="21"/>
<wire x1="3.9878" y1="0.4064" x2="4.8768" y2="5.1308" width="0.254" layer="21"/>
<wire x1="3.5306" y1="4.7498" x2="2.8448" y2="1.0414" width="0.254" layer="21"/>
<wire x1="2.54" y1="0.635" x2="3.683" y2="0" width="0.254" layer="21"/>
<wire x1="3.175" y1="5.08" x2="4.0386" y2="5.7658" width="0.254" layer="21"/>
<wire x1="4.0386" y1="5.7658" x2="3.2766" y2="6.35" width="0.254" layer="21"/>
<wire x1="2.413" y1="-0.635" x2="3.683" y2="0" width="0.254" layer="21"/>
<wire x1="2.4638" y1="-1.1938" x2="3.81" y2="-0.508" width="0.254" layer="21"/>
<wire x1="3.81" y1="-0.508" x2="2.921" y2="-5.4102" width="0.254" layer="21"/>
<wire x1="2.4638" y1="-1.1938" x2="1.8034" y2="-4.7498" width="0.254" layer="21"/>
<wire x1="1.4732" y1="-5.08" x2="2.2352" y2="-5.9944" width="0.254" layer="21"/>
<wire x1="2.2352" y1="-5.9944" x2="1.8288" y2="-6.35" width="0.254" layer="21"/>
<wire x1="1.8034" y1="-4.7498" x2="2.5908" y2="-5.6896" width="0.254" layer="21"/>
<wire x1="2.5908" y1="-5.6896" x2="2.921" y2="-5.4102" width="0.254" layer="21"/>
<wire x1="-2.54" y1="-0.635" x2="-3.683" y2="0" width="0.254" layer="21"/>
<wire x1="-3.175" y1="-5.08" x2="-4.0386" y2="-5.7658" width="0.254" layer="21"/>
<wire x1="-4.0386" y1="-5.7658" x2="-3.2766" y2="-6.35" width="0.254" layer="21"/>
<wire x1="-3.2766" y1="-6.35" x2="1.8288" y2="-6.35" width="0.254" layer="21"/>
<wire x1="-3.175" y1="-5.08" x2="1.4732" y2="-5.08" width="0.254" layer="21"/>
<wire x1="2.413" y1="-0.635" x2="-2.54" y2="-0.635" width="0.254" layer="21"/>
<wire x1="-2.413" y1="0.635" x2="-3.683" y2="0" width="0.254" layer="21"/>
<wire x1="-2.413" y1="0.635" x2="2.54" y2="0.635" width="0.254" layer="21"/>
<wire x1="-1.4732" y1="5.08" x2="-2.2352" y2="5.9944" width="0.254" layer="21"/>
<wire x1="-2.2352" y1="5.9944" x2="-1.8288" y2="6.35" width="0.254" layer="21"/>
<wire x1="-1.8288" y1="6.35" x2="3.2766" y2="6.35" width="0.254" layer="21"/>
<wire x1="-1.4732" y1="5.08" x2="3.175" y2="5.08" width="0.254" layer="21"/>
<wire x1="-3.302" y1="-5.715" x2="1.397" y2="-5.715" width="1.016" layer="21"/>
<wire x1="2.413" y1="-4.699" x2="3.048" y2="-1.524" width="1.016" layer="21"/>
<wire x1="-2.667" y1="0" x2="2.667" y2="0" width="1.016" layer="21"/>
<wire x1="2.54" y1="-5.207" x2="2.667" y2="-5.08" width="0.762" layer="21"/>
<wire x1="3.556" y1="1.397" x2="4.191" y2="4.699" width="1.016" layer="21"/>
<wire x1="4.445" y1="5.08" x2="4.572" y2="4.953" width="0.508" layer="21"/>
<wire x1="3.175" y1="1.27" x2="3.81" y2="0.889" width="0.6604" layer="21"/>
<wire x1="3.302" y1="5.715" x2="-1.397" y2="5.715" width="1.016" layer="21"/>
<wire x1="-2.032" y1="5.842" x2="-1.651" y2="6.223" width="0.4064" layer="21"/>
<wire x1="-3.5306" y1="-4.7498" x2="-4.4196" y2="-5.461" width="0.254" layer="21"/>
<wire x1="-4.4196" y1="-5.461" x2="-4.8768" y2="-5.1308" width="0.254" layer="21"/>
<wire x1="-2.8448" y1="-1.0414" x2="-3.9878" y2="-0.4064" width="0.254" layer="21"/>
<wire x1="-3.9878" y1="-0.4064" x2="-4.8768" y2="-5.1308" width="0.254" layer="21"/>
<wire x1="-3.5306" y1="-4.7498" x2="-2.8448" y2="-1.0414" width="0.254" layer="21"/>
<wire x1="-2.4638" y1="1.1938" x2="-3.81" y2="0.508" width="0.254" layer="21"/>
<wire x1="-3.81" y1="0.508" x2="-2.921" y2="5.4102" width="0.254" layer="21"/>
<wire x1="-2.4638" y1="1.1938" x2="-1.8034" y2="4.7498" width="0.254" layer="21"/>
<wire x1="-1.8034" y1="4.7498" x2="-2.5908" y2="5.6896" width="0.254" layer="21"/>
<wire x1="-2.5908" y1="5.6896" x2="-2.921" y2="5.4102" width="0.254" layer="21"/>
<wire x1="-2.413" y1="4.699" x2="-3.048" y2="1.524" width="1.016" layer="21"/>
<wire x1="-2.54" y1="5.207" x2="-2.667" y2="5.08" width="0.762" layer="21"/>
<wire x1="-3.556" y1="-1.397" x2="-4.191" y2="-4.699" width="1.016" layer="21"/>
<wire x1="-4.445" y1="-5.08" x2="-4.572" y2="-4.953" width="0.508" layer="21"/>
<wire x1="-3.175" y1="-1.27" x2="-3.81" y2="-0.889" width="0.6604" layer="21"/>
<circle x="4.572" y="-5.715" radius="0.381" width="0.762" layer="21"/>
<pad name="1" x="-5.08" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="2" x="-2.54" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="3" x="0" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="4" x="2.54" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="5" x="5.08" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="6" x="5.08" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="7" x="2.54" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="8" x="0" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="9" x="-2.54" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="10" x="-5.08" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<text x="-6.096" y="9.144" size="1.778" layer="25" ratio="10">&gt;NAME</text>
<text x="-6.096" y="-10.922" size="1.778" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="1.524" y1="-6.223" x2="2.159" y2="-5.715" layer="21"/>
<rectangle x1="3.175" y1="-1.27" x2="3.683" y2="-0.762" layer="21"/>
<rectangle x1="3.048" y1="-0.254" x2="3.429" y2="0.254" layer="21"/>
<rectangle x1="-3.429" y1="-0.127" x2="-3.048" y2="0.127" layer="21"/>
<rectangle x1="-3.683" y1="0.762" x2="-3.175" y2="1.27" layer="21"/>
<rectangle x1="4.318" y1="-5.842" x2="4.826" y2="-5.588" layer="21"/>
</package>
</packages>
<symbols>
<symbol name="7SEG-CA">
<wire x1="-5.08" y1="7.62" x2="-5.08" y2="-7.62" width="0.4064" layer="94"/>
<wire x1="-5.08" y1="-7.62" x2="5.08" y2="-7.62" width="0.4064" layer="94"/>
<wire x1="5.08" y1="-7.62" x2="5.08" y2="7.62" width="0.4064" layer="94"/>
<wire x1="5.08" y1="7.62" x2="-5.08" y2="7.62" width="0.4064" layer="94"/>
<wire x1="-2.4892" y1="5.5118" x2="2.9972" y2="5.5118" width="0.8128" layer="94"/>
<wire x1="-3.5052" y1="-5.5118" x2="2.0066" y2="-5.5118" width="0.8128" layer="94"/>
<wire x1="-2.9972" y1="4.4958" x2="-3.5052" y2="0.9906" width="0.8128" layer="94"/>
<wire x1="-3.5052" y1="-0.508" x2="-3.9878" y2="-4.4958" width="0.8128" layer="94"/>
<wire x1="3.5052" y1="4.4958" x2="2.9972" y2="0.9906" width="0.8128" layer="94"/>
<wire x1="2.9972" y1="-0.508" x2="2.4892" y2="-4.4958" width="0.8128" layer="94"/>
<wire x1="-2.4892" y1="0" x2="2.0066" y2="0" width="0.8128" layer="94"/>
<circle x="3.81" y="-5.588" radius="0.1016" width="0.8128" layer="94"/>
<text x="-6.4008" y="5.3848" size="1.27" layer="94">a</text>
<text x="-6.4008" y="2.794" size="1.27" layer="94">b</text>
<text x="-6.4008" y="0.4064" size="1.27" layer="94">c</text>
<text x="-6.4008" y="-2.2098" size="1.27" layer="94">d</text>
<text x="-6.4008" y="-4.8006" size="1.27" layer="94">e</text>
<text x="5.4102" y="5.4102" size="1.27" layer="94">f</text>
<text x="5.4102" y="3.2004" size="1.27" layer="94">g</text>
<text x="5.4102" y="0.4064" size="1.27" layer="94">P</text>
<text x="5.4102" y="-2.2098" size="1.27" layer="94">A</text>
<text x="-5.08" y="8.255" size="1.778" layer="95">&gt;NAME</text>
<text x="-5.08" y="-10.16" size="1.778" layer="96">&gt;VALUE</text>
<text x="5.4102" y="-4.8006" size="1.27" layer="94">A</text>
<pin name="C" x="-7.62" y="0" visible="off" length="short" direction="pas"/>
<pin name="B" x="-7.62" y="2.54" visible="off" length="short" direction="pas"/>
<pin name="A" x="-7.62" y="5.08" visible="off" length="short" direction="pas"/>
<pin name="D" x="-7.62" y="-2.54" visible="off" length="short" direction="pas"/>
<pin name="E" x="-7.62" y="-5.08" visible="off" length="short" direction="pas"/>
<pin name="CA1" x="7.62" y="-2.54" visible="off" length="short" direction="pas" rot="R180"/>
<pin name="DP" x="7.62" y="0" visible="off" length="short" direction="pas" rot="R180"/>
<pin name="G" x="7.62" y="2.54" visible="off" length="short" direction="pas" rot="R180"/>
<pin name="F" x="7.62" y="5.08" visible="off" length="short" direction="pas" rot="R180"/>
<pin name="CA2" x="7.62" y="-5.08" visible="off" length="short" direction="pas" rot="R180"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="7SEG-CA" prefix="D" uservalue="yes">
<description>7-segment &lt;b&gt;DISPLAY&lt;/b&gt;&lt;p&gt;
single anode, 13 mm</description>
<gates>
<gate name="G$1" symbol="7SEG-CA" x="0" y="0"/>
</gates>
<devices>
<device name="" package="7SEG-13">
<connects>
<connect gate="G$1" pin="A" pad="7"/>
<connect gate="G$1" pin="B" pad="6"/>
<connect gate="G$1" pin="C" pad="4"/>
<connect gate="G$1" pin="CA1" pad="3"/>
<connect gate="G$1" pin="CA2" pad="8"/>
<connect gate="G$1" pin="D" pad="2"/>
<connect gate="G$1" pin="DP" pad="5"/>
<connect gate="G$1" pin="E" pad="1"/>
<connect gate="G$1" pin="F" pad="9"/>
<connect gate="G$1" pin="G" pad="10"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="con-molex">
<description>&lt;b&gt;Molex Connectors&lt;/b&gt;&lt;p&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
<package name="22-23-2051">
<description>.100" (2.54mm) Center Header - 5 Pin</description>
<wire x1="-6.35" y1="3.175" x2="6.35" y2="3.175" width="0.254" layer="21"/>
<wire x1="6.35" y1="3.175" x2="6.35" y2="1.27" width="0.254" layer="21"/>
<wire x1="6.35" y1="1.27" x2="6.35" y2="-3.175" width="0.254" layer="21"/>
<wire x1="6.35" y1="-3.175" x2="-6.35" y2="-3.175" width="0.254" layer="21"/>
<wire x1="-6.35" y1="-3.175" x2="-6.35" y2="1.27" width="0.254" layer="21"/>
<wire x1="-6.35" y1="1.27" x2="-6.35" y2="3.175" width="0.254" layer="21"/>
<wire x1="-6.35" y1="1.27" x2="6.35" y2="1.27" width="0.254" layer="21"/>
<pad name="1" x="-5.08" y="0" drill="1" shape="long" rot="R90"/>
<pad name="2" x="-2.54" y="0" drill="1" shape="long" rot="R90"/>
<pad name="3" x="0" y="0" drill="1" shape="long" rot="R90"/>
<pad name="4" x="2.54" y="0" drill="1" shape="long" rot="R90"/>
<pad name="5" x="5.08" y="0" drill="1" shape="long" rot="R90"/>
<text x="-6.35" y="3.81" size="1.016" layer="25" ratio="10">&gt;NAME</text>
<text x="-6.35" y="-5.08" size="1.016" layer="27" ratio="10">&gt;VALUE</text>
</package>
</packages>
<symbols>
<symbol name="MV">
<wire x1="1.27" y1="0" x2="0" y2="0" width="0.6096" layer="94"/>
<text x="2.54" y="-0.762" size="1.524" layer="95">&gt;NAME</text>
<text x="-0.762" y="1.397" size="1.778" layer="96">&gt;VALUE</text>
<pin name="S" x="-2.54" y="0" visible="off" length="short" direction="pas"/>
</symbol>
<symbol name="M">
<wire x1="1.27" y1="0" x2="0" y2="0" width="0.6096" layer="94"/>
<text x="2.54" y="-0.762" size="1.524" layer="95">&gt;NAME</text>
<pin name="S" x="-2.54" y="0" visible="off" length="short" direction="pas"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="22-23-2051" prefix="X">
<description>.100" (2.54mm) Center Header - 5 Pin</description>
<gates>
<gate name="-1" symbol="MV" x="0" y="5.08" addlevel="always" swaplevel="1"/>
<gate name="-2" symbol="M" x="0" y="2.54" addlevel="always" swaplevel="1"/>
<gate name="-3" symbol="M" x="0" y="0" addlevel="always" swaplevel="1"/>
<gate name="-4" symbol="M" x="0" y="-2.54" addlevel="always" swaplevel="1"/>
<gate name="-5" symbol="M" x="0" y="-5.08" addlevel="always" swaplevel="1"/>
</gates>
<devices>
<device name="" package="22-23-2051">
<connects>
<connect gate="-1" pin="S" pad="1"/>
<connect gate="-2" pin="S" pad="2"/>
<connect gate="-3" pin="S" pad="3"/>
<connect gate="-4" pin="S" pad="4"/>
<connect gate="-5" pin="S" pad="5"/>
</connects>
<technologies>
<technology name="">
<attribute name="MF" value="MOLEX" constant="no"/>
<attribute name="MPN" value="22-23-2051" constant="no"/>
<attribute name="OC_FARNELL" value="1462952" constant="no"/>
<attribute name="OC_NEWARK" value="38C9178" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
</libraries>
<attributes>
</attributes>
<variantdefs>
</variantdefs>
<classes>
<class number="0" name="default" width="0" drill="0">
</class>
</classes>
<parts>
<part name="D1" library="special" deviceset="7SEG-CA" device="" value="TDSG5150"/>
<part name="D2" library="special" deviceset="7SEG-CA" device="" value="TDSG5150"/>
<part name="X1" library="con-molex" deviceset="22-23-2051" device=""/>
<part name="X2" library="con-molex" deviceset="22-23-2051" device=""/>
<part name="X3" library="con-molex" deviceset="22-23-2051" device=""/>
<part name="X4" library="con-molex" deviceset="22-23-2051" device=""/>
</parts>
<sheets>
<sheet>
<plain>
</plain>
<instances>
<instance part="D1" gate="G$1" x="30.48" y="30.48"/>
<instance part="D2" gate="G$1" x="68.58" y="30.48"/>
<instance part="X1" gate="-1" x="25.4" y="58.42" rot="R90"/>
<instance part="X1" gate="-2" x="27.94" y="58.42" rot="R90"/>
<instance part="X1" gate="-3" x="30.48" y="58.42" rot="R90"/>
<instance part="X1" gate="-4" x="33.02" y="58.42" rot="R90"/>
<instance part="X1" gate="-5" x="35.56" y="58.42" rot="R90"/>
<instance part="X2" gate="-1" x="35.56" y="10.16" rot="R270"/>
<instance part="X2" gate="-2" x="33.02" y="10.16" rot="R270"/>
<instance part="X2" gate="-3" x="30.48" y="10.16" rot="R270"/>
<instance part="X2" gate="-4" x="27.94" y="10.16" rot="R270"/>
<instance part="X2" gate="-5" x="25.4" y="10.16" rot="R270"/>
<instance part="X3" gate="-1" x="63.5" y="58.42" rot="R90"/>
<instance part="X3" gate="-2" x="66.04" y="58.42" rot="R90"/>
<instance part="X3" gate="-3" x="68.58" y="58.42" rot="R90"/>
<instance part="X3" gate="-4" x="71.12" y="58.42" rot="R90"/>
<instance part="X3" gate="-5" x="73.66" y="58.42" rot="R90"/>
<instance part="X4" gate="-1" x="73.66" y="10.16" rot="R270"/>
<instance part="X4" gate="-2" x="71.12" y="10.16" rot="R270"/>
<instance part="X4" gate="-3" x="68.58" y="10.16" rot="R270"/>
<instance part="X4" gate="-4" x="66.04" y="10.16" rot="R270"/>
<instance part="X4" gate="-5" x="63.5" y="10.16" rot="R270"/>
</instances>
<busses>
</busses>
<nets>
<net name="N$1" class="0">
<segment>
<pinref part="D1" gate="G$1" pin="E"/>
<wire x1="22.86" y1="25.4" x2="20.32" y2="25.4" width="0.1524" layer="91"/>
<wire x1="20.32" y1="25.4" x2="20.32" y2="20.32" width="0.1524" layer="91"/>
<wire x1="20.32" y1="20.32" x2="25.4" y2="20.32" width="0.1524" layer="91"/>
<pinref part="X2" gate="-5" pin="S"/>
<wire x1="25.4" y1="20.32" x2="25.4" y2="12.7" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$2" class="0">
<segment>
<pinref part="D1" gate="G$1" pin="D"/>
<wire x1="22.86" y1="27.94" x2="17.78" y2="27.94" width="0.1524" layer="91"/>
<wire x1="17.78" y1="27.94" x2="17.78" y2="17.78" width="0.1524" layer="91"/>
<wire x1="17.78" y1="17.78" x2="27.94" y2="17.78" width="0.1524" layer="91"/>
<pinref part="X2" gate="-4" pin="S"/>
<wire x1="27.94" y1="17.78" x2="27.94" y2="12.7" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$3" class="0">
<segment>
<pinref part="D2" gate="G$1" pin="E"/>
<wire x1="60.96" y1="25.4" x2="58.42" y2="25.4" width="0.1524" layer="91"/>
<wire x1="58.42" y1="25.4" x2="58.42" y2="20.32" width="0.1524" layer="91"/>
<wire x1="58.42" y1="20.32" x2="63.5" y2="20.32" width="0.1524" layer="91"/>
<pinref part="X4" gate="-5" pin="S"/>
<wire x1="63.5" y1="20.32" x2="63.5" y2="12.7" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$4" class="0">
<segment>
<pinref part="D2" gate="G$1" pin="D"/>
<wire x1="60.96" y1="27.94" x2="55.88" y2="27.94" width="0.1524" layer="91"/>
<wire x1="55.88" y1="27.94" x2="55.88" y2="17.78" width="0.1524" layer="91"/>
<wire x1="55.88" y1="17.78" x2="66.04" y2="17.78" width="0.1524" layer="91"/>
<pinref part="X4" gate="-4" pin="S"/>
<wire x1="66.04" y1="17.78" x2="66.04" y2="12.7" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$5" class="0">
<segment>
<pinref part="D1" gate="G$1" pin="C"/>
<wire x1="22.86" y1="30.48" x2="15.24" y2="30.48" width="0.1524" layer="91"/>
<wire x1="15.24" y1="30.48" x2="15.24" y2="15.24" width="0.1524" layer="91"/>
<wire x1="15.24" y1="15.24" x2="33.02" y2="15.24" width="0.1524" layer="91"/>
<pinref part="X2" gate="-2" pin="S"/>
<wire x1="33.02" y1="15.24" x2="33.02" y2="12.7" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$6" class="0">
<segment>
<pinref part="D1" gate="G$1" pin="DP"/>
<wire x1="38.1" y1="30.48" x2="40.64" y2="30.48" width="0.1524" layer="91"/>
<wire x1="40.64" y1="30.48" x2="40.64" y2="15.24" width="0.1524" layer="91"/>
<wire x1="40.64" y1="15.24" x2="35.56" y2="15.24" width="0.1524" layer="91"/>
<pinref part="X2" gate="-1" pin="S"/>
<wire x1="35.56" y1="15.24" x2="35.56" y2="12.7" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$7" class="0">
<segment>
<pinref part="D2" gate="G$1" pin="C"/>
<wire x1="60.96" y1="30.48" x2="53.34" y2="30.48" width="0.1524" layer="91"/>
<wire x1="53.34" y1="30.48" x2="53.34" y2="15.24" width="0.1524" layer="91"/>
<wire x1="53.34" y1="15.24" x2="71.12" y2="15.24" width="0.1524" layer="91"/>
<pinref part="X4" gate="-2" pin="S"/>
<wire x1="71.12" y1="15.24" x2="71.12" y2="12.7" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$8" class="0">
<segment>
<pinref part="X4" gate="-1" pin="S"/>
<wire x1="73.66" y1="12.7" x2="73.66" y2="15.24" width="0.1524" layer="91"/>
<wire x1="73.66" y1="15.24" x2="78.74" y2="15.24" width="0.1524" layer="91"/>
<wire x1="78.74" y1="15.24" x2="78.74" y2="30.48" width="0.1524" layer="91"/>
<pinref part="D2" gate="G$1" pin="DP"/>
<wire x1="78.74" y1="30.48" x2="76.2" y2="30.48" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$9" class="0">
<segment>
<pinref part="D1" gate="G$1" pin="G"/>
<wire x1="38.1" y1="33.02" x2="43.18" y2="33.02" width="0.1524" layer="91"/>
<wire x1="43.18" y1="33.02" x2="43.18" y2="45.72" width="0.1524" layer="91"/>
<pinref part="X1" gate="-1" pin="S"/>
<wire x1="43.18" y1="45.72" x2="25.4" y2="45.72" width="0.1524" layer="91"/>
<wire x1="25.4" y1="45.72" x2="25.4" y2="55.88" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$10" class="0">
<segment>
<pinref part="D2" gate="G$1" pin="G"/>
<wire x1="76.2" y1="33.02" x2="81.28" y2="33.02" width="0.1524" layer="91"/>
<wire x1="81.28" y1="33.02" x2="81.28" y2="45.72" width="0.1524" layer="91"/>
<pinref part="X3" gate="-1" pin="S"/>
<wire x1="81.28" y1="45.72" x2="63.5" y2="45.72" width="0.1524" layer="91"/>
<wire x1="63.5" y1="45.72" x2="63.5" y2="55.88" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$11" class="0">
<segment>
<pinref part="D1" gate="G$1" pin="F"/>
<wire x1="38.1" y1="35.56" x2="40.64" y2="35.56" width="0.1524" layer="91"/>
<wire x1="40.64" y1="35.56" x2="40.64" y2="43.18" width="0.1524" layer="91"/>
<pinref part="X1" gate="-2" pin="S"/>
<wire x1="40.64" y1="43.18" x2="27.94" y2="43.18" width="0.1524" layer="91"/>
<wire x1="27.94" y1="43.18" x2="27.94" y2="55.88" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$12" class="0">
<segment>
<pinref part="D2" gate="G$1" pin="F"/>
<wire x1="76.2" y1="35.56" x2="78.74" y2="35.56" width="0.1524" layer="91"/>
<wire x1="78.74" y1="35.56" x2="78.74" y2="43.18" width="0.1524" layer="91"/>
<wire x1="78.74" y1="43.18" x2="66.04" y2="43.18" width="0.1524" layer="91"/>
<pinref part="X3" gate="-2" pin="S"/>
<wire x1="66.04" y1="43.18" x2="66.04" y2="55.88" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$13" class="0">
<segment>
<pinref part="D1" gate="G$1" pin="CA1"/>
<wire x1="38.1" y1="27.94" x2="45.72" y2="27.94" width="0.1524" layer="91"/>
<wire x1="45.72" y1="27.94" x2="45.72" y2="48.26" width="0.1524" layer="91"/>
<pinref part="X1" gate="-3" pin="S"/>
<wire x1="45.72" y1="48.26" x2="30.48" y2="48.26" width="0.1524" layer="91"/>
<wire x1="30.48" y1="48.26" x2="30.48" y2="55.88" width="0.1524" layer="91"/>
<pinref part="D1" gate="G$1" pin="CA2"/>
<wire x1="38.1" y1="25.4" x2="45.72" y2="25.4" width="0.1524" layer="91"/>
<wire x1="45.72" y1="25.4" x2="45.72" y2="27.94" width="0.1524" layer="91"/>
<junction x="45.72" y="27.94"/>
</segment>
</net>
<net name="N$14" class="0">
<segment>
<pinref part="D2" gate="G$1" pin="CA1"/>
<wire x1="76.2" y1="27.94" x2="83.82" y2="27.94" width="0.1524" layer="91"/>
<wire x1="83.82" y1="27.94" x2="83.82" y2="48.26" width="0.1524" layer="91"/>
<pinref part="X3" gate="-3" pin="S"/>
<wire x1="83.82" y1="48.26" x2="68.58" y2="48.26" width="0.1524" layer="91"/>
<wire x1="68.58" y1="48.26" x2="68.58" y2="55.88" width="0.1524" layer="91"/>
<pinref part="D2" gate="G$1" pin="CA2"/>
<wire x1="76.2" y1="25.4" x2="83.82" y2="25.4" width="0.1524" layer="91"/>
<wire x1="83.82" y1="25.4" x2="83.82" y2="27.94" width="0.1524" layer="91"/>
<junction x="83.82" y="27.94"/>
</segment>
</net>
<net name="N$15" class="0">
<segment>
<pinref part="D1" gate="G$1" pin="A"/>
<wire x1="22.86" y1="35.56" x2="20.32" y2="35.56" width="0.1524" layer="91"/>
<wire x1="20.32" y1="35.56" x2="20.32" y2="50.8" width="0.1524" layer="91"/>
<pinref part="X1" gate="-4" pin="S"/>
<wire x1="20.32" y1="50.8" x2="33.02" y2="50.8" width="0.1524" layer="91"/>
<wire x1="33.02" y1="50.8" x2="33.02" y2="55.88" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$16" class="0">
<segment>
<pinref part="D2" gate="G$1" pin="A"/>
<wire x1="60.96" y1="35.56" x2="58.42" y2="35.56" width="0.1524" layer="91"/>
<wire x1="58.42" y1="35.56" x2="58.42" y2="50.8" width="0.1524" layer="91"/>
<pinref part="X3" gate="-4" pin="S"/>
<wire x1="58.42" y1="50.8" x2="71.12" y2="50.8" width="0.1524" layer="91"/>
<wire x1="71.12" y1="50.8" x2="71.12" y2="55.88" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$17" class="0">
<segment>
<pinref part="D1" gate="G$1" pin="B"/>
<wire x1="22.86" y1="33.02" x2="17.78" y2="33.02" width="0.1524" layer="91"/>
<wire x1="17.78" y1="33.02" x2="17.78" y2="53.34" width="0.1524" layer="91"/>
<pinref part="X1" gate="-5" pin="S"/>
<wire x1="17.78" y1="53.34" x2="35.56" y2="53.34" width="0.1524" layer="91"/>
<wire x1="35.56" y1="53.34" x2="35.56" y2="55.88" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$18" class="0">
<segment>
<pinref part="D2" gate="G$1" pin="B"/>
<wire x1="60.96" y1="33.02" x2="55.88" y2="33.02" width="0.1524" layer="91"/>
<wire x1="55.88" y1="33.02" x2="55.88" y2="53.34" width="0.1524" layer="91"/>
<pinref part="X3" gate="-5" pin="S"/>
<wire x1="55.88" y1="53.34" x2="73.66" y2="53.34" width="0.1524" layer="91"/>
<wire x1="73.66" y1="53.34" x2="73.66" y2="55.88" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$19" class="0">
<segment>
<pinref part="X2" gate="-3" pin="S"/>
<wire x1="30.48" y1="12.7" x2="30.48" y2="17.78" width="0.1524" layer="91"/>
</segment>
</net>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
</eagle>
