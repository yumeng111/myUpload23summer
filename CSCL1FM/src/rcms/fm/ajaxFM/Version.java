/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package rcms.fm.ajaxFM;

/**
 *
 * @author banicz
 */
public class Version {

    static final int MAJOR = 1;
    static final int MINOR = 1;
    static final int PATCH = 1;
    
    public String getString(){ return MAJOR + "." + MINOR  + "." + PATCH; }
    public int getMajor(){ return MAJOR; }
    public int getMinor(){ return MINOR; }
    public int getPatch(){ return PATCH; }

}
